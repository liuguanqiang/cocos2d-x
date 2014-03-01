/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
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

#include "UICheckBox.h"

NS_CC_BEGIN

namespace gui {

static const int BACKGROUNDBOX_RENDERER_Z = (-1);
static const int BACKGROUNDBOXSELECTED_RENDERER_Z = (-1);
static const int FRONTCROSS_RENDERER_Z = (-1);
static const int BACKGROUNDBOXDISABLED_RENDERER_Z = (-1);
static const int FRONTCROSSDISABLED_RENDERER_Z = (-1);

CheckBox::CheckBox():
_backGroundBoxRenderer(NULL),
_backGroundSelectedBoxRenderer(NULL),
_frontCrossRenderer(NULL),
_backGroundBoxDisabledRenderer(NULL),
_frontCrossDisabledRenderer(NULL),
_isSelected(true),
_checkBoxEventListener(NULL),
_checkBoxEventSelector(NULL),
_backGroundTexType(UI_TEX_TYPE_LOCAL),
_backGroundSelectedTexType(UI_TEX_TYPE_LOCAL),
_frontCrossTexType(UI_TEX_TYPE_LOCAL),
_backGroundDisabledTexType(UI_TEX_TYPE_LOCAL),
_frontCrossDisabledTexType(UI_TEX_TYPE_LOCAL),
_backGroundFileName(""),
_backGroundSelectedFileName(""),
_frontCrossFileName(""),
_backGroundDisabledFileName(""),
_frontCrossDisabledFileName("")
{
}

CheckBox::~CheckBox()
{
    _checkBoxEventListener = NULL;
    _checkBoxEventSelector = NULL;
}

CheckBox* CheckBox::create()
{
    CheckBox* widget = new CheckBox();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool CheckBox::init()
{
    if (Widget::init())
    {
        setSelectedState(false);
        return true;
    }
    return false;
}

void CheckBox::initRenderer()
{
    _backGroundBoxRenderer = CCSprite::create();
    _backGroundSelectedBoxRenderer = CCSprite::create();
    _frontCrossRenderer = CCSprite::create();
    _backGroundBoxDisabledRenderer = CCSprite::create();
    _frontCrossDisabledRenderer = CCSprite::create();
    
    CCNodeRGBA::addChild(_backGroundBoxRenderer, BACKGROUNDBOX_RENDERER_Z, -1);
    CCNodeRGBA::addChild(_backGroundSelectedBoxRenderer, BACKGROUNDBOXSELECTED_RENDERER_Z, -1);
    CCNodeRGBA::addChild(_frontCrossRenderer, FRONTCROSS_RENDERER_Z, -1);
    CCNodeRGBA::addChild(_backGroundBoxDisabledRenderer, BACKGROUNDBOXDISABLED_RENDERER_Z, -1);
    CCNodeRGBA::addChild(_frontCrossDisabledRenderer, FRONTCROSSDISABLED_RENDERER_Z, -1);
}

void CheckBox::loadTextures(const char *backGround, const char *backGroundSelected, const char *cross,const char* backGroundDisabled,const char* frontCrossDisabled,TextureResType texType)
{
    loadTextureBackGround(backGround,texType);
    loadTextureBackGroundSelected(backGroundSelected,texType);
    loadTextureFrontCross(cross,texType);
    loadTextureBackGroundDisabled(backGroundDisabled,texType);
    loadTextureFrontCrossDisabled(frontCrossDisabled,texType);
}

void CheckBox::loadTextureBackGround(const char *backGround,TextureResType texType)
{
    if (!backGround || strcmp(backGround, "") == 0)
    {
        return;
    }
    _backGroundFileName = backGround;
    _backGroundTexType = texType;
    switch (_backGroundTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            _backGroundBoxRenderer->initWithFile(backGround);
            break;
        case UI_TEX_TYPE_PLIST:
            _backGroundBoxRenderer->initWithSpriteFrameName(backGround);
            break;
        default:
            break;
    }

    dynamic_cast<CCRGBAProtocol*>(_backGroundBoxRenderer)->setColor(getColor());
    dynamic_cast<CCRGBAProtocol*>(_backGroundBoxRenderer)->setOpacity(getOpacity());    
    updateDisplayedColor(getColor());
    updateDisplayedOpacity(getOpacity());
    updateAnchorPoint();
    backGroundTextureScaleChangedWithSize();
}

void CheckBox::loadTextureBackGroundSelected(const char *backGroundSelected,TextureResType texType)
{
    if (!backGroundSelected || strcmp(backGroundSelected, "") == 0)
    {
        return;
    }
    _backGroundSelectedFileName = backGroundSelected;
    _backGroundSelectedTexType = texType;
    switch (_backGroundSelectedTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            _backGroundSelectedBoxRenderer->initWithFile(backGroundSelected);
            break;
        case UI_TEX_TYPE_PLIST:
            _backGroundSelectedBoxRenderer->initWithSpriteFrameName(backGroundSelected);
            break;
        default:
            break;
    }
    dynamic_cast<CCRGBAProtocol*>(_backGroundSelectedBoxRenderer)->setColor(getColor());
    dynamic_cast<CCRGBAProtocol*>(_backGroundSelectedBoxRenderer)->setOpacity(getOpacity()); 
    updateAnchorPoint();
    backGroundSelectedTextureScaleChangedWithSize();
}

void CheckBox::loadTextureFrontCross(const char *cross,TextureResType texType)
{
    if (!cross || strcmp(cross, "") == 0)
    {
        return;
    }
    _frontCrossFileName = cross;
    _frontCrossTexType = texType;
    switch (_frontCrossTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            _frontCrossRenderer->initWithFile(cross);
            break;
        case UI_TEX_TYPE_PLIST:
            _frontCrossRenderer->initWithSpriteFrameName(cross);
            break;
        default:
            break;
    }
    dynamic_cast<CCRGBAProtocol*>(_frontCrossRenderer)->setColor(getColor());
    dynamic_cast<CCRGBAProtocol*>(_frontCrossRenderer)->setOpacity(getOpacity()); 
    updateAnchorPoint();
    frontCrossTextureScaleChangedWithSize();
}

void CheckBox::loadTextureBackGroundDisabled(const char *backGroundDisabled,TextureResType texType)
{
    if (!backGroundDisabled || strcmp(backGroundDisabled, "") == 0)
    {
        return;
    }
    _backGroundDisabledFileName = backGroundDisabled;
    _backGroundDisabledTexType = texType;
    switch (_backGroundDisabledTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            _backGroundBoxDisabledRenderer->initWithFile(backGroundDisabled);
            break;
        case UI_TEX_TYPE_PLIST:
            _backGroundBoxDisabledRenderer->initWithSpriteFrameName(backGroundDisabled);
            break;
        default:
            break;
    }
    dynamic_cast<CCRGBAProtocol*>(_backGroundBoxDisabledRenderer)->setColor(getColor());
    dynamic_cast<CCRGBAProtocol*>(_backGroundBoxDisabledRenderer)->setOpacity(getOpacity()); 
    updateAnchorPoint();
    backGroundDisabledTextureScaleChangedWithSize();
}

void CheckBox::loadTextureFrontCrossDisabled(const char *frontCrossDisabled,TextureResType texType)
{
    if (!frontCrossDisabled || strcmp(frontCrossDisabled, "") == 0)
    {
        return;
    }
    _frontCrossDisabledFileName = frontCrossDisabled;
    _frontCrossDisabledTexType = texType;
    switch (_frontCrossDisabledTexType)
    {
        case UI_TEX_TYPE_LOCAL:
            _frontCrossDisabledRenderer->initWithFile(frontCrossDisabled);
            break;
        case UI_TEX_TYPE_PLIST:
            _frontCrossDisabledRenderer->initWithSpriteFrameName(frontCrossDisabled);
            break;
        default:
            break;
    }
    dynamic_cast<CCRGBAProtocol*>(_frontCrossDisabledRenderer)->setColor(getColor());
    dynamic_cast<CCRGBAProtocol*>(_frontCrossDisabledRenderer)->setOpacity(getOpacity()); 
    updateAnchorPoint();
    frontCrossDisabledTextureScaleChangedWithSize();
}

void CheckBox::onTouchEnded(CCTouch *touch, CCEvent *unused_event)
{
    _touchEndPos = touch->getLocation();
    if (_focus)
    {
        releaseUpEvent();
        if (_isSelected){
            setSelectedState(false);
            unSelectedEvent();
        }
        else
        {
            setSelectedState(true);
            selectedEvent();
        }
    }
    setFocused(false);
    Widget* widgetParent = getWidgetParent();
    if (widgetParent)
    {
        widgetParent->checkChildInfo(2,this,_touchEndPos);
    }
}

void CheckBox::onPressStateChangedToNormal()
{
    _backGroundBoxRenderer->setVisible(true);
    _backGroundSelectedBoxRenderer->setVisible(false);
    _backGroundBoxDisabledRenderer->setVisible(false);
    _frontCrossDisabledRenderer->setVisible(false);
}

void CheckBox::onPressStateChangedToPressed()
{
    _backGroundBoxRenderer->setVisible(false);
    _backGroundSelectedBoxRenderer->setVisible(true);
    _backGroundBoxDisabledRenderer->setVisible(false);
    _frontCrossDisabledRenderer->setVisible(false);
}

void CheckBox::onPressStateChangedToDisabled()
{
    _backGroundBoxRenderer->setVisible(false);
    _backGroundSelectedBoxRenderer->setVisible(false);
    _backGroundBoxDisabledRenderer->setVisible(true);
    _frontCrossRenderer->setVisible(false);
    if (_isSelected)
    {
        _frontCrossDisabledRenderer->setVisible(true);
    }
}

void CheckBox::setSelectedState(bool selected)
{
    if (selected == _isSelected)
    {
        return;
    }
    _isSelected = selected;
    _frontCrossRenderer->setVisible(_isSelected);
}

bool CheckBox::getSelectedState()
{
    return _isSelected;
}

void CheckBox::selectedEvent()
{
    if (_checkBoxEventListener && _checkBoxEventSelector)
    {
        (_checkBoxEventListener->*_checkBoxEventSelector)(this,CHECKBOX_STATE_EVENT_SELECTED);
    }
}

void CheckBox::unSelectedEvent()
{
    if (_checkBoxEventListener && _checkBoxEventSelector)
    {
        (_checkBoxEventListener->*_checkBoxEventSelector)(this,CHECKBOX_STATE_EVENT_UNSELECTED);
    }
}

void CheckBox::addEventListenerCheckBox(CCObject *target, SEL_SelectedStateEvent selector)
{
    _checkBoxEventListener = target;
    _checkBoxEventSelector = selector;
}

void CheckBox::setFlipX(bool flipX)
{
    _backGroundBoxRenderer->setFlipX(flipX);
    _backGroundSelectedBoxRenderer->setFlipX(flipX);
    _frontCrossRenderer->setFlipX(flipX);
    _backGroundBoxDisabledRenderer->setFlipX(flipX);
    _frontCrossDisabledRenderer->setFlipX(flipX);
}

void CheckBox::setFlipY(bool flipY)
{
    _backGroundBoxRenderer->setFlipY(flipY);
    _backGroundSelectedBoxRenderer->setFlipY(flipY);
    _frontCrossRenderer->setFlipY(flipY);
    _backGroundBoxDisabledRenderer->setFlipY(flipY);
    _frontCrossDisabledRenderer->setFlipY(flipY);
}

bool CheckBox::isFlipX()
{
    return _backGroundBoxRenderer->isFlipX();
}

bool CheckBox::isFlipY()
{
    return _backGroundBoxRenderer->isFlipY();
}

void CheckBox::setAnchorPoint(const CCPoint &pt)
{
    Widget::setAnchorPoint(pt);
    _backGroundBoxRenderer->setAnchorPoint(pt);
    _backGroundSelectedBoxRenderer->setAnchorPoint(pt);
    _backGroundBoxDisabledRenderer->setAnchorPoint(pt);
    _frontCrossRenderer->setAnchorPoint(pt);
    _frontCrossDisabledRenderer->setAnchorPoint(pt);
}

void CheckBox::onSizeChanged()
{
    Widget::onSizeChanged();
    backGroundTextureScaleChangedWithSize();
    backGroundSelectedTextureScaleChangedWithSize();
    frontCrossTextureScaleChangedWithSize();
    backGroundDisabledTextureScaleChangedWithSize();
    frontCrossDisabledTextureScaleChangedWithSize();
}

const CCSize& CheckBox::getContentSize() const
{
    return _backGroundBoxRenderer->getContentSize();
}

CCNode* CheckBox::getVirtualRenderer()
{
    return _backGroundBoxRenderer;
}

void CheckBox::backGroundTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _backGroundBoxRenderer->setScale(1.0f);
        _size = _backGroundBoxRenderer->getContentSize();
    }
    else
    {
        CCSize textureSize = _backGroundBoxRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _backGroundBoxRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _backGroundBoxRenderer->setScaleX(scaleX);
        _backGroundBoxRenderer->setScaleY(scaleY);
    }
}

void CheckBox::backGroundSelectedTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _backGroundSelectedBoxRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _backGroundSelectedBoxRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _backGroundSelectedBoxRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _backGroundSelectedBoxRenderer->setScaleX(scaleX);
        _backGroundSelectedBoxRenderer->setScaleY(scaleY);
    }
}

void CheckBox::frontCrossTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _frontCrossRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _frontCrossRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _frontCrossRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _frontCrossRenderer->setScaleX(scaleX);
        _frontCrossRenderer->setScaleY(scaleY);
    }
}

void CheckBox::backGroundDisabledTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _backGroundBoxDisabledRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _backGroundBoxDisabledRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _backGroundBoxDisabledRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _backGroundBoxDisabledRenderer->setScaleX(scaleX);
        _backGroundBoxDisabledRenderer->setScaleY(scaleY);
    }
}

void CheckBox::frontCrossDisabledTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _frontCrossDisabledRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _frontCrossDisabledRenderer->getContentSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _frontCrossDisabledRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _frontCrossDisabledRenderer->setScaleX(scaleX);
        _frontCrossDisabledRenderer->setScaleY(scaleY);
    }
}

std::string CheckBox::getDescription() const
{
    return "CheckBox";
}

Widget* CheckBox::createCloneInstance()
{
    return CheckBox::create();
}

void CheckBox::copySpecialProperties(Widget *widget)
{
    CheckBox* checkBox = dynamic_cast<CheckBox*>(widget);
    if (checkBox)
    {
        loadTextureBackGround(checkBox->_backGroundFileName.c_str(), checkBox->_backGroundTexType);
        loadTextureBackGroundSelected(checkBox->_backGroundSelectedFileName.c_str(), checkBox->_backGroundSelectedTexType);
        loadTextureFrontCross(checkBox->_frontCrossFileName.c_str(), checkBox->_frontCrossTexType);
        loadTextureBackGroundDisabled(checkBox->_backGroundDisabledFileName.c_str(), checkBox->_backGroundDisabledTexType);
        loadTextureFrontCrossDisabled(checkBox->_frontCrossDisabledFileName.c_str(), checkBox->_frontCrossDisabledTexType);
        setSelectedState(checkBox->_isSelected);
    }
}

void CheckBox::setOpacity(GLubyte opacity)
{
    _displayedOpacity = _realOpacity = opacity;
    CCRGBAProtocol* _backGroundBoxRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_backGroundBoxRenderer);
    if (_backGroundBoxRendererProtocol)
    {
        _backGroundBoxRendererProtocol->setOpacity(opacity);
    }  
    CCRGBAProtocol* _backGroundSelectedBoxRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_backGroundSelectedBoxRenderer);
    if (_backGroundSelectedBoxRendererProtocol)
    {
        _backGroundSelectedBoxRendererProtocol->setOpacity(opacity);
    }  
    CCRGBAProtocol* _backGroundBoxDisabledRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_backGroundBoxDisabledRenderer);
    if (_backGroundBoxDisabledRendererProtocol)
    {
        _backGroundBoxDisabledRendererProtocol->setOpacity(opacity);
    }  
    CCRGBAProtocol* _frontCrossRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_frontCrossRenderer);
    if (_frontCrossRendererProtocol)
    {
        _frontCrossRendererProtocol->setOpacity(opacity);
    }  
    CCRGBAProtocol* _frontCrossDisabledRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_frontCrossDisabledRenderer);
    if (_frontCrossDisabledRendererProtocol)
    {
        _frontCrossDisabledRendererProtocol->setOpacity(opacity);
    }  
}

void CheckBox::setColor(const ccColor3B& color)
{
    _displayedColor = _realColor = color;
    CCRGBAProtocol* _backGroundBoxRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_backGroundBoxRenderer);
    if (_backGroundBoxRendererProtocol)
    {
        _backGroundBoxRendererProtocol->setColor(color);
    }  
    CCRGBAProtocol* _backGroundSelectedBoxRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_backGroundSelectedBoxRenderer);
    if (_backGroundSelectedBoxRendererProtocol)
    {
        _backGroundSelectedBoxRendererProtocol->setColor(color);
    }  
    CCRGBAProtocol* _backGroundBoxDisabledRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_backGroundBoxDisabledRenderer);
    if (_backGroundBoxDisabledRendererProtocol)
    {
        _backGroundBoxDisabledRendererProtocol->setColor(color);
    }  
    CCRGBAProtocol* _frontCrossRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_frontCrossRenderer);
    if (_frontCrossRendererProtocol)
    {
        _frontCrossRendererProtocol->setColor(color);
    }  
    CCRGBAProtocol* _frontCrossDisabledRendererProtocol = dynamic_cast<CCRGBAProtocol*>(_frontCrossDisabledRenderer);
    if (_frontCrossDisabledRendererProtocol)
    {
        _frontCrossDisabledRendererProtocol->setColor(color);
    }  
}

}

NS_CC_END