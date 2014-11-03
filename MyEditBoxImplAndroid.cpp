/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "MyEditBoxImplAndroid.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "MyEditBox.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"


NS_CC_EXT_BEGIN

MyEditBoxImpl* __createSystemEditBox(MyEditBox* pEditBox)
{
    return new MyEditBoxImplAndroid(pEditBox);
}

MyEditBoxImplAndroid::MyEditBoxImplAndroid(MyEditBox* pEditText)
: MyEditBoxImpl(pEditText)
, m_pLabel(NULL)
, m_pLabelPlaceHolder(NULL)
, m_eEditBoxInputMode(kEditBoxInputModeSingleLine)
, m_eEditBoxInputFlag(kEditBoxInputFlagInitialCapsAllCharacters)
, m_eKeyboardReturnType(kKeyboardReturnTypeDefault)
, m_colText(ccWHITE)
, m_colPlaceHolder(ccGRAY)
, m_nMaxLength(-1)
{
    
}

MyEditBoxImplAndroid::~MyEditBoxImplAndroid()
{
	
}

void MyEditBoxImplAndroid::doAnimationWhenKeyboardMove(float duration, float distance)
{ // don't need to be implemented on android platform.
	
}

static const int CC_EDIT_BOX_PADDING = 5;

bool MyEditBoxImplAndroid::initWithSize(const CCSize& size)
{
    int fontSize = getFontSizeAccordingHeightJni(size.height-12);
    m_pLabel = CCLabelTTF::create("", "", size.height-12);
	m_pLabel->setDimensions(size);
	m_pLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
	// align the text vertically center
    m_pLabel->setAnchorPoint(ccp(0, 0.5f));
    m_pLabel->setPosition(ccp(CC_EDIT_BOX_PADDING, size.height / 2.0f));
    m_pLabel->setColor(m_colText);
    m_pEditBox->addChild(m_pLabel);
	
    m_pLabelPlaceHolder = CCLabelTTF::create("", "", size.height-12);
	// align the text vertically center
    m_pLabelPlaceHolder->setAnchorPoint(ccp(0, 0.5f));
    m_pLabelPlaceHolder->setPosition(ccp(CC_EDIT_BOX_PADDING, size.height / 2.0f));
    m_pLabelPlaceHolder->setVisible(false);
    m_pLabelPlaceHolder->setColor(m_colPlaceHolder);
    m_pEditBox->addChild(m_pLabelPlaceHolder);
    
    m_EditSize = size;
    return true;
}

void MyEditBoxImplAndroid::setFont(const char* pFontName, int fontSize)
{
	if(m_pLabel != NULL) {
		m_pLabel->setFontName(pFontName);
		m_pLabel->setFontSize(fontSize);
	}
	
	if(m_pLabelPlaceHolder != NULL) {
		m_pLabelPlaceHolder->setFontName(pFontName);
		m_pLabelPlaceHolder->setFontSize(fontSize);
	}
}

void MyEditBoxImplAndroid::setFontColor(const ccColor3B& color)
{
    m_colText = color;
    m_pLabel->setColor(color);
}

void MyEditBoxImplAndroid::setPlaceholderFont(const char* pFontName, int fontSize)
{
	if(m_pLabelPlaceHolder != NULL) {
		m_pLabelPlaceHolder->setFontName(pFontName);
		m_pLabelPlaceHolder->setFontSize(fontSize);
	}
}

void MyEditBoxImplAndroid::setPlaceholderFontColor(const ccColor3B& color)
{
    m_colPlaceHolder = color;
    m_pLabelPlaceHolder->setColor(color);
}

void MyEditBoxImplAndroid::setInputMode(EditBoxInputMode inputMode)
{
    m_eEditBoxInputMode = inputMode;
}

void MyEditBoxImplAndroid::setMaxLength(int maxLength)
{
    m_nMaxLength = maxLength;
}

int MyEditBoxImplAndroid::getMaxLength()
{
    return m_nMaxLength;
}

void MyEditBoxImplAndroid::setInputFlag(EditBoxInputFlag inputFlag)
{
    m_eEditBoxInputFlag = inputFlag;
}

void MyEditBoxImplAndroid::setReturnType(KeyboardReturnType returnType)
{
    m_eKeyboardReturnType = returnType;
}

bool MyEditBoxImplAndroid::isEditing()
{
    return false;
}

void MyEditBoxImplAndroid::setText(const char* pText)
{
    if (pText != NULL)
    {
        m_strText = pText;
		
        if (m_strText.length() > 0)
        {
            m_pLabelPlaceHolder->setVisible(false);
			
            std::string strToShow;
			
            if (kEditBoxInputFlagPassword == m_eEditBoxInputFlag)
            {
                long length = cc_utf8_strlen(m_strText.c_str(), -1);
                for (long i = 0; i < length; i++)
                {
                    strToShow.append("\u25CF");
                }
            }
            else
            {
                strToShow = m_strText;
            }

			m_pLabel->setString(strToShow.c_str());

			// Clip the text width to fit to the text box
			float fMaxWidth = m_EditSize.width - CC_EDIT_BOX_PADDING * 2;
			CCRect clippingRect = m_pLabel->getTextureRect();
			if(clippingRect.size.width > fMaxWidth) {
				clippingRect.size.width = fMaxWidth;
				m_pLabel->setTextureRect(clippingRect);
			}

        }
        else
        {
            m_pLabelPlaceHolder->setVisible(true);
            m_pLabel->setString("");
        }
		
    }
}

const char*  MyEditBoxImplAndroid::getText(void)
{
    return m_strText.c_str();
}

void MyEditBoxImplAndroid::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        m_strPlaceHolder = pText;
        if (m_strPlaceHolder.length() > 0 && m_strText.length() == 0)
        {
            m_pLabelPlaceHolder->setVisible(true);
        }
		
        m_pLabelPlaceHolder->setString(m_strPlaceHolder.c_str());
    }
}

void MyEditBoxImplAndroid::setPosition(const CCPoint& pos)
{ // don't need to be implemented on android platform.
	
}

void MyEditBoxImplAndroid::setVisible(bool visible)
{ // don't need to be implemented on android platform.

}

void MyEditBoxImplAndroid::setContentSize(const CCSize& size)
{ // don't need to be implemented on android platform.
	
}

void MyEditBoxImplAndroid::setAnchorPoint(const CCPoint& anchorPoint)
{ // don't need to be implemented on android platform.
	
}

void MyEditBoxImplAndroid::visit(void)
{ // don't need to be implemented on android platform.
    
}

void MyEditBoxImplAndroid::onEnter(void)
{ // don't need to be implemented on android platform.
    
}

static void editBoxCallbackFunc(const char* pText, void* ctx)
{
    MyEditBoxImplAndroid* thiz = (MyEditBoxImplAndroid*)ctx;
    thiz->setText(pText);
	
    if (thiz->getDelegate() != NULL)
    {
        thiz->getDelegate()->editBoxTextChanged(thiz->getCCEditBox(), thiz->getText());
        thiz->getDelegate()->editBoxEditingDidEnd(thiz->getCCEditBox());
        thiz->getDelegate()->editBoxReturn(thiz->getCCEditBox());
    }
    
    MyEditBox* pEditBox = thiz->getCCEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "changed",pEditBox);
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "ended",pEditBox);
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "return",pEditBox);
    }
}

void MyEditBoxImplAndroid::openKeyboard()
{
    if (m_pDelegate != NULL)
    {
        m_pDelegate->editBoxEditingDidBegin(m_pEditBox);
    }
    MyEditBox* pEditBox = this->getCCEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CCScriptEngineProtocol* pEngine = cocos2d::CCScriptEngineManager::sharedManager()->getScriptEngine();
        pEngine->executeEvent(pEditBox->getScriptEditBoxHandler(), "began",pEditBox);
    }
	
    showEditTextDialogJNI(  m_strPlaceHolder.c_str(),
						  m_strText.c_str(),
						  m_eEditBoxInputMode,
						  m_eEditBoxInputFlag,
						  m_eKeyboardReturnType,
						  m_nMaxLength,
						  editBoxCallbackFunc,
						  (void*)this  );
	
}

void MyEditBoxImplAndroid::closeKeyboard()
{
	
}

NS_CC_EXT_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) */

