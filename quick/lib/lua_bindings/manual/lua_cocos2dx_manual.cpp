/****************************************************************************
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
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
#include "lua_cocos2dx_manual.hpp"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "CCLuaEngine.h"
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#endif
#include "event/CCScriptEventDispatcher.h"

static int tolua_cocos2d_Scheduler_scheduleScriptFunc(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    Scheduler* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Scheduler",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Scheduler*>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Scheduler_scheduleScriptFunc'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    if (3 == argc) {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
            !tolua_isboolean(tolua_S,4,0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler =  toluafix_ref_function(tolua_S,2,0);
        float interval = (float)  tolua_tonumber(tolua_S,3,0);
        bool  paused   = (bool)  tolua_toboolean(tolua_S,4,0);
        unsigned int tolua_ret = (unsigned int)  self->scheduleScriptFunc(handler,interval,paused);
        tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
        return 1;
    }
    
    CCLOG("'scheduleScriptFunc' has wrong number of arguments: %d, was expecting %d\n", argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'scheduleScriptFunc'.",&tolua_err);
    return 0;
#endif
}


static int tolua_cocos2d_Scheduler_unscheduleScriptEntry(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    Scheduler* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Scheduler",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Scheduler*>(tolua_tousertype(tolua_S,1,0));
    
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Scheduler_unscheduleScriptEntry'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    if (1 == argc) {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        
        unsigned int scheduleScriptEntryID = ((unsigned int)  tolua_tonumber(tolua_S,2,0));
        self->unscheduleScriptEntry(scheduleScriptEntryID);
        return 0;
    }
    
    CCLOG("'unscheduleScriptEntry' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'unscheduleScriptEntry'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Sequence_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    
    tolua_Error tolua_err;
    
#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertable(tolua_S,1,"cc.Sequence",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    if(argc > 0)
    {
        Vector<FiniteTimeAction*> array;

        if (1 == argc && tolua_istable(tolua_S, 2, 0, &tolua_err))
        {
            luaval_to_ccvector(tolua_S, 2, &array);
        }
        else
        {
            uint32_t i = 1;
            while (i <= argc)
            {
#if COCOS2D_DEBUG >= 1
                if (!tolua_isusertype(tolua_S, 1 + i, "cc.FiniteTimeAction", 0, &tolua_err))
                    goto tolua_lerror;
#endif
                
                cocos2d::FiniteTimeAction* item = static_cast<cocos2d::FiniteTimeAction*>(tolua_tousertype(tolua_S, 1 + i, NULL));
                if (NULL != item)
                {
                    array.pushBack(item);
                    ++i;
                }
            }
        }
        
        cocos2d::Sequence* tolua_ret = cocos2d::Sequence::create(array);
        //issue 2433 uncheck
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.Sequence");
        return 1;
    }
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_CallFunc_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S,1,"cc.CallFunc",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 1 || argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if(!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err))
            goto tolua_lerror;
#endif
        
        LUA_FUNCTION handler =  toluafix_ref_function(tolua_S,2,0);
        
        bool hasExtraData = false;
        int  ref  = 0;
        if (argc == 2)
        {
#if COCOS2D_DEBUG >= 1
            if(!tolua_istable(tolua_S, 3, 0, &tolua_err))
                goto tolua_lerror;
#endif
            lua_pushvalue(tolua_S, 3);
            ref = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
            hasExtraData = true;
        }
        LuaCallFunc* tolua_ret = new LuaCallFunc();
        tolua_ret->initWithFunction([=](void* self,Node* target){
            int callbackHandler =  ScriptHandlerMgr::getInstance()->getObjectHandler((void*)tolua_ret, ScriptHandlerMgr::HandlerType::CALLFUNC);
            
            if (0 != callbackHandler)
            {
                LuaStack* stack = LuaEngine::getInstance()->getLuaStack();
                int argNums = 1;
                if (nullptr != target)
                {
                    stack->pushObject(target, "cc.Node");
                }
                else
                {
                    stack->pushNil();
                }
                
                if (hasExtraData)
                {
                    lua_rawgeti(tolua_S, LUA_REGISTRYINDEX,ref);
                    if (lua_istable(tolua_S, -1))
                    {
                        argNums += 1;
                    }
                    else
                    {
                        lua_pop(tolua_S, 1);
                    }
                }
                stack->executeFunctionByHandler(callbackHandler, argNums);
                if (hasExtraData)
                {
                    luaL_unref(tolua_S, LUA_REGISTRYINDEX,ref);
                }
                stack->clean();
            }
        });
        tolua_ret->autorelease();
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)tolua_ret, handler, ScriptHandlerMgr::HandlerType::CALLFUNC);
        
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CallFunc");
        return 1;
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
    
}

static int tolua_Cocos2d_Node_scheduleUpdate(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    Node* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_Cocos2d_Node_scheduleUpdate'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->scheduleUpdate();
        return 0;
    }
    
    CCLOG("'scheduleUpdate' has wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'scheduleUpdate'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Node_unscheduleUpdate(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    Node* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Node_unscheduleUpdate'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (0 == argc)
    {
        self->unscheduleUpdate();
        return 0;
    }
    
    CCLOG("'unscheduleUpdate' has wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'unscheduleUpdate'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_Node_setContentSize(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = NULL;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_setContentSize'", NULL);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    
    if (1 == argc)
    {
        cocos2d::Size size;
        ok &= luaval_to_size(tolua_S, 2, &size);
        if (!ok)
            return 0;
        
        cobj->setContentSize(size);
        return 0;
    }
    else if(2 == argc)
    {
        double width;
        ok &= luaval_to_number(tolua_S, 2,&width);
        
        if (!ok)
            return 0;
        
        double height;
        ok &= luaval_to_number(tolua_S, 3,&height);
        
        if (!ok)
            return 0;
        
        cobj->setContentSize(Size(width, height));
        return 0;
    }
    
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setContentSize",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_setContentSize'.",&tolua_err);
#endif
    return 0;
}

int tolua_cocos2d_Node_setAnchorPoint(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::Node* cobj = NULL;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Node*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Node_setAnchorPoint'", NULL);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    
    if (1 == argc)
    {
        cocos2d::Vec2 pt;
        ok &= luaval_to_vec2(tolua_S, 2, &pt);
        if (!ok)
            return 0;
        
        cobj->setAnchorPoint(pt);
        return 0;
    }
    else if(2 == argc)
    {
        double x;
        ok &= luaval_to_number(tolua_S, 2,&x);
        
        if (!ok)
            return 0;
        
        double y;
        ok &= luaval_to_number(tolua_S, 3,&y);
        
        if (!ok)
            return 0;
        
        cobj->setAnchorPoint(cocos2d::Vec2(x,y));
        return 0;
    }
    
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAnchorPoint",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Node_setAnchorPoint'.",&tolua_err);
#endif
    return 0;
}

static int tolua_cocos2d_Node_getPosition(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    Node* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_Node_getPosition'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc >= 0 && argc <= 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S,2,1,&tolua_err) || !tolua_isnumber(tolua_S,3,1,&tolua_err) )
            goto tolua_lerror;
#endif
        float x = (float)  tolua_tonumber(tolua_S,2,0);
        float y = (float)  tolua_tonumber(tolua_S,3,0);
        
        self->getPosition(&x,&y);
        
        tolua_pushnumber(tolua_S,(lua_Number)x);
        tolua_pushnumber(tolua_S,(lua_Number)y);
        
        return 2;
    }
    
    CCLOG("'getPosition' function in Node has wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getPosition'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Node_addNodeEventListener(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        (tolua_isvaluenil(tolua_S,3,&tolua_err) || !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err)) ||
//        !tolua_isnumber(tolua_S,4,1,&tolua_err) ||
//        !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
        !tolua_isnoobj(tolua_S,6,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        int event = ((int)  tolua_tonumber(tolua_S,2,0));
        LUA_FUNCTION listener = (toluafix_ref_function(tolua_S,3,0));
        int tag = 0;
        if (lua_isnumber(tolua_S,4)) {
            tag = ((int)  tolua_tonumber(tolua_S,4,0));
        }
        int priority = 0;
        if (lua_isnumber(tolua_S,5)) {
            priority = ((int)  tolua_tonumber(tolua_S,5,0));
        }
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addScriptEventListener'", NULL);
#endif
        {
            int tolua_ret = (int)  self->addScriptEventListener(event,listener,tag,priority);
            tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'addNodeEventListener'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_removeNodeEventListener(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        int handle = ((int)  tolua_tonumber(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeScriptEventListener'", NULL);
#endif
        {
            self->getScriptEventDispatcher()->removeScriptEventListener(handle);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'removeNodeEventListener'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_removeNodeEventListenersByEvent(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        int event = ((int)  tolua_tonumber(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeScriptEventListenersByEvent'", NULL);
#endif
        {
            self->getScriptEventDispatcher()->removeScriptEventListenersByEvent(event);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'removeNodeEventListenersByEvent'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_removeNodeEventListenersByTag(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        int tag = ((int)  tolua_tonumber(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeScriptEventListenersByTag'", NULL);
#endif
        {
            self->getScriptEventDispatcher()->removeScriptEventListenersByTag(tag);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'removeNodeEventListenersByTag'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_removeAllNodeEventListeners(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'removeAllScriptEventListeners'", NULL);
#endif
        {
            self->getScriptEventDispatcher()->removeAllScriptEventListeners();
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'removeAllNodeEventListeners'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_hasNodeEventListener(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        int event = ((int)  tolua_tonumber(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'hasScriptEventListener'", NULL);
#endif
        {
            bool tolua_ret = (bool)  self->getScriptEventDispatcher()->hasScriptEventListener(event);
            tolua_pushboolean(tolua_S,(bool)tolua_ret);
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'hasNodeEventListener'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_isTouchEnabled(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isTouchEnabled'", NULL);
#endif
        {
            bool tolua_ret = (bool)  self->isTouchEnabled();
            tolua_pushboolean(tolua_S,(bool)tolua_ret);
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'isTouchEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_setTouchEnabled(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTouchEnabled'", NULL);
#endif
        {
            self->setTouchEnabled(value);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTouchEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_isTouchSwallowEnabled(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isTouchSwallowEnabled'", NULL);
#endif
        {
            bool tolua_ret = (bool)  self->isTouchSwallowEnabled();
            tolua_pushboolean(tolua_S,(bool)tolua_ret);
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'isTouchSwallowEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_setTouchSwallowEnabled(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTouchSwallowEnabled'", NULL);
#endif
        {
            self->setTouchSwallowEnabled(value);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTouchSwallowEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_isTouchCaptureEnabled(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isTouchCaptureEnabled'", NULL);
#endif
        {
            bool tolua_ret = (bool)  self->isTouchCaptureEnabled();
            tolua_pushboolean(tolua_S,(bool)tolua_ret);
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'isTouchCaptureEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_setTouchCaptureEnabled(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = static_cast<cocos2d::Node*>(tolua_tousertype(tolua_S,1,0));
        bool value = ((bool)  tolua_toboolean(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTouchCaptureEnabled'", NULL);
#endif
        {
            self->setTouchCaptureEnabled(value);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTouchCaptureEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_setTouchMode(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,3,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = (Node*)  tolua_tousertype(tolua_S,1,0);
        int mode = ((int)  tolua_tonumber(tolua_S,2,0));
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTouchMode'", NULL);
#endif
        {
            self->setTouchMode(mode);
        }
    }
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTouchMode'.",&tolua_err);
    return 0;
#endif
}

static int tolua_Cocos2d_Node_getTouchMode(lua_State* tolua_S)
{
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (
        !tolua_isusertype(tolua_S,1,"cc.Node",0,&tolua_err) ||
        !tolua_isnoobj(tolua_S,2,&tolua_err)
        )
        goto tolua_lerror;
    else
#endif
    {
        Node* self = (Node*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
        if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTouchMode'", NULL);
#endif
        {
            int tolua_ret = (int)  self->getTouchMode();
            tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
        }
    }
    return 1;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getTouchMode'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_Node_setKeyboardEnabled(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    int argc = 0;
    Node* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Node*>(tolua_tousertype(L,1,0));
    
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(L,"invalid 'self' in function 'lua_cocos2dx_Node_setKeyboardEnabled'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(L) - 1;
    if (1 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isboolean(L, 2, 0, &tolua_err))
            goto tolua_lerror;
#endif
        bool enabled = tolua_toboolean(L, 2, 0);
        self->Node::setKeyboardEnabled(enabled);
        return 0;
    }
    
    CCLOG("'setKeyboardEnabled' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'setKeyboardEnabled'.",&tolua_err);
    return 0;
#endif
}

static int lua_cocos2dx_Node_isKeyboardEnabled(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    int argc = 0;
    Node* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
	if (!tolua_isusertype(L,1,"cc.Node",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::Node*>(tolua_tousertype(L,1,0));
    
#if COCOS2D_DEBUG >= 1
	if (nullptr == self)
    {
		tolua_error(L,"invalid 'self' in function 'lua_cocos2dx_Node_isKeyboardEnabled'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(L) - 1;
    if (0 == argc)
    {
        bool ret = self->Node::isKeyboardEnabled();
        tolua_pushboolean(L, ret);
        return 1;
    }
    
    CCLOG("'isKeyboardEnabled' has wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'isKeyboardEnabled'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_Spawn_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    
    tolua_Error tolua_err;
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.Spawn",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc > 0)
    {
        Vector<FiniteTimeAction*> array;        
        uint32_t i = 1;
        
        if (1 == argc && tolua_istable(tolua_S, 2, 0, &tolua_err))
        {
            luaval_to_ccvector(tolua_S, 2, &array);
        }
        else
        {
            while (i <= argc)
            {
#if COCOS2D_DEBUG >= 1
                if (!tolua_isusertype(tolua_S, 1 + i, "cc.FiniteTimeAction", 0, &tolua_err))
                    goto tolua_lerror;
#endif
                
                cocos2d::FiniteTimeAction* item = static_cast<cocos2d::FiniteTimeAction*>(tolua_tousertype(tolua_S, 1 + i, NULL));
                if (NULL != item)
                {
                    array.pushBack(item);
                    ++i;
                }
            }
        }
        
        cocos2d::Spawn * tolua_ret = cocos2d::Spawn::create(array);
        int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.Spawn");
        return 1;
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

int lua_cocos2d_CardinalSplineBy_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    bool ok = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.CardinalSplineBy",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 3)
    {
        double dur = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &dur);
        if (!ok)
            return 0;
        
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num);
        if (!ok)
            return 0;
        
        double ten = 0.0;
        ok &= luaval_to_number(tolua_S, 4, &ten);
        if (!ok)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }
        
        if (num > 0)
        {
            PointArray* points = PointArray::create(num);
            
            if (NULL == points)
            {
                CC_SAFE_DELETE_ARRAY(arr);
                return 0;
            }
            
            for( int i = 0; i < num; i++) {
                points->addControlPoint(arr[i]);
            }
            
            CC_SAFE_DELETE_ARRAY(arr);
            CardinalSplineBy* tolua_ret = CardinalSplineBy::create(dur, points, ten);
            if (NULL != tolua_ret)
            {
                int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
                int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
                toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CardinalSplineBy");
                return 1;
            }
        }
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 3);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_CatmullRomBy_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    bool ok = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.CatmullRomBy",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 2)
    {
        double dur = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &dur);
        if (!ok)
            return 0;
        
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num);
        if (!ok)
            return 0;
        
        if (num > 0)
        {
            PointArray* points = PointArray::create(num);
            
            if (NULL == points)
            {
                CC_SAFE_DELETE_ARRAY(arr);
                return 0;
            }
            
            for( int i = 0; i < num; i++) {
                points->addControlPoint(arr[i]);
            }
            
            CC_SAFE_DELETE_ARRAY(arr);
            CatmullRomBy* tolua_ret = CatmullRomBy::create(dur, points);
            if (NULL != tolua_ret)
            {
                int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
                int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
                toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CatmullRomBy");
                return 1;
            }
        }
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_CatmullRomTo_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    bool ok = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.CatmullRomTo",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 2)
    {
        double dur = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &dur);
        if (!ok)
            return 0;
        
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num);
        if (!ok)
            return 0;
        
        if (num > 0)
        {
            PointArray* points = PointArray::create(num);
            
            if (NULL == points)
            {
                CC_SAFE_DELETE_ARRAY(arr);
                return 0;
            }
            
            for( int i = 0; i < num; i++) {
                points->addControlPoint(arr[i]);
            }
            
            CC_SAFE_DELETE_ARRAY(arr);
            CatmullRomTo* tolua_ret = CatmullRomTo::create(dur, points);
            if (NULL != tolua_ret)
            {
                int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
                int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
                toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.CatmullRomTo");
                return 1;
            }
        }
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_BezierBy_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    bool ok = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.BezierBy",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 2)
    {
        double t = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &t);
        if (!ok)
            return 0;
        
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num);
        if (!ok)
            return 0;
        
        if (num < 3)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }
        
        ccBezierConfig config;
        config.controlPoint_1 = arr[0];
        config.controlPoint_2 = arr[1];
        config.endPosition = arr[2];
        CC_SAFE_DELETE_ARRAY(arr);
        
        BezierBy* tolua_ret = BezierBy::create(t, config);
        if (NULL != tolua_ret)
        {
            int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
            int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
            toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.BezierBy");
            return 1;
        }
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

int tolua_cocos2d_BezierTo_create(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    bool ok = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.BezierTo",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 2)
    {
        double t = 0.0;
        ok &= luaval_to_number(tolua_S, 2, &t);
        if (!ok)
            return 0;
        
        int num = 0;
        cocos2d::Vec2 *arr = NULL;
        ok &= luaval_to_array_of_vec2(tolua_S, 3, &arr, &num);
        if (!ok)
            return 0;
        
        if (num < 3)
        {
            CC_SAFE_DELETE_ARRAY(arr);
            return 0;
        }
        
        ccBezierConfig config;
        config.controlPoint_1 = arr[0];
        config.controlPoint_2 = arr[1];
        config.endPosition = arr[2];
        CC_SAFE_DELETE_ARRAY(arr);
        
        BezierTo* tolua_ret = BezierTo::create(t, config);
        if (NULL != tolua_ret)
        {
            int nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
            int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
            toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.BezierTo");
            return 1;
        }
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2d_DrawNode_drawPolygon(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    DrawNode* self = nullptr;
  
    tolua_Error tolua_err;
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.DrawNode",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<cocos2d::DrawNode*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
	if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_DrawNode_drawPolygon'\n", NULL);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    if (5 == argc)
    {
#if COCOS2D_DEBUG >= 1
        if(
        !tolua_istable(tolua_S, 2, 0, &tolua_err)  ||
        !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
        !tolua_istable(tolua_S, 4, 0,&tolua_err)   ||
        !tolua_isnumber(tolua_S, 5, 0, &tolua_err) ||
        !tolua_istable(tolua_S,6, 0,&tolua_err) )
        {
            goto tolua_lerror;
        }
#endif
        size_t size = lua_tonumber(tolua_S, 3);
        if ( size > 0 )
        {
            cocos2d::Vec2* points = new cocos2d::Vec2[size];
            if (NULL == points)
                return 0;
            
            for (int i = 0; i < size; i++)
            {
                lua_pushnumber(tolua_S,i + 1);
                lua_gettable(tolua_S,2);
                if (!tolua_istable(tolua_S,-1, 0, &tolua_err))
                {
                    CC_SAFE_DELETE_ARRAY(points);
#if COCOS2D_DEBUG >= 1
                    goto tolua_lerror;
#endif
                }
                
                if(!luaval_to_vec2(tolua_S, lua_gettop(tolua_S), &points[i]))
                {
                    lua_pop(tolua_S, 1);
                    CC_SAFE_DELETE_ARRAY(points);
                    return 0;
                }
                lua_pop(tolua_S, 1);
            }
            
            Color4F fillColor;
            if (!luaval_to_color4f(tolua_S, 4, &fillColor))
            {
                CC_SAFE_DELETE_ARRAY(points);
                return 0;
            }
            
            float borderWidth  = (float)tolua_tonumber(tolua_S, 5, 0);
            
            Color4F borderColor;
            if (!luaval_to_color4f(tolua_S, 6, &borderColor))
            {
                CC_SAFE_DELETE_ARRAY(points);
                return 0;
            }
            
            self->drawPolygon(points, (int)size, fillColor, borderWidth, borderColor);
            CC_SAFE_DELETE_ARRAY(points);
            return 0;
        }        
    }
    
    CCLOG("'drawPolygon' has wrong number of arguments: %d, was expecting %d\n", argc, 5);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'drawPolygon'.",&tolua_err);
    return 0;
#endif
}

// setBlendFunc
template<class T>
static int tolua_cocos2dx_setBlendFunc(lua_State* tolua_S,const char* className)
{
    if (NULL == tolua_S || NULL == className || strlen(className) == 0)
        return 0;
    
    int argc = 0;
    T* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,className,0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = static_cast<T*>(tolua_tousertype(tolua_S,1,0));
    
    argc = lua_gettop(tolua_S) - 1;
    if (2 == argc)
    {
        GLenum src, dst;
        if (!luaval_to_int32(tolua_S, 2, (int32_t*)&src))
            return 0;
        
        if (!luaval_to_int32(tolua_S, 3, (int32_t*)&dst))
            return 0;
        
        BlendFunc blendFunc = {src, dst};
        self->setBlendFunc(blendFunc);
        return 0;
    }

    
    CCLOG("'setBlendFunc' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setBlendFunc'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_Sprite_setBlendFunc(lua_State* tolua_S)
{
    return tolua_cocos2dx_setBlendFunc<Sprite>(tolua_S,"cc.Sprite");
}

static int tolua_cocos2dx_SpriteBatchNode_setBlendFunc(lua_State* tolua_S)
{
    return tolua_cocos2dx_setBlendFunc<SpriteBatchNode>(tolua_S,"cc.SpriteBatchNode");
}

static int tolua_cocos2dx_MotionStreak_setBlendFunc(lua_State* tolua_S)
{
    return tolua_cocos2dx_setBlendFunc<MotionStreak>(tolua_S,"cc.MotionStreak");
}

static int tolua_cocos2dx_AtlasNode_setBlendFunc(lua_State* tolua_S)
{
    return tolua_cocos2dx_setBlendFunc<AtlasNode>(tolua_S,"cc.AtlasNode");
}

static int tolua_cocos2dx_ParticleBatchNode_setBlendFunc(lua_State* tolua_S)
{
    return tolua_cocos2dx_setBlendFunc<ParticleBatchNode>(tolua_S,"cc.ParticleBatchNode");
}

static int tolua_cocos2dx_LayerColor_setBlendFunc(lua_State* tolua_S)
{
    return tolua_cocos2dx_setBlendFunc<LayerColor>(tolua_S,"cc.LayerColor");
}

static int tolua_cocos2dx_ParticleSystem_setBlendFunc(lua_State* tolua_S) 
{
    return tolua_cocos2dx_setBlendFunc<ParticleSystem>(tolua_S,"cc.ParticleSystem");
}

static int tolua_cocos2dx_DrawNode_setBlendFunc(lua_State* tolua_S) 
{
    return tolua_cocos2dx_setBlendFunc<DrawNode>(tolua_S,"cc.DrawNode");
}

static int tolua_cocos2dx_LayerMultiplex_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    Vector<Layer*> arg0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.LayerMultiplex", 0, &tolua_err))  goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
 
    if (argc > 0)
    {
        if (luavals_variadic_to_ccvector(tolua_S, argc, &arg0))
        {
            LayerMultiplex* tolua_ret =  LayerMultiplex::createWithArray(arg0);
            int  nID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
            int* pLuaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
            toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"cc.LayerMultiplex");
            return 1;
        }
        else
        {
            CCLOG("error in tolua_cocos2dx_LayerMultiplex_create \n");
            return 0;
        }
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_UserDefault_getInstance(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.UserDefault",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if(0 == argc)
    {
        UserDefault* tolua_ret = (UserDefault*)  UserDefault::getInstance();
        tolua_pushusertype(tolua_S,(void*)tolua_ret,"cc.UserDefault");
        return 1;
    }
    
    CCLOG("'getInstance' has wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getInstance'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_GLProgram_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    bool ok = false;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.GLProgram",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if(2 == argc)
    {
        const char *arg0, *arg1;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp); arg1 = arg1_tmp.c_str();
        
        GLProgram* tolua_ret = new GLProgram();
        if (nullptr == tolua_ret)
            return 0;        

        tolua_ret->autorelease();
        tolua_ret->initWithFilenames(arg0, arg1);
        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.GLProgram");
        return 1;
        
    }
    
    CCLOG("'create' function of GLProgram wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}


static int tolua_cocos2d_GLProgram_getProgram(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    GLProgram* self = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.GLProgram",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = (GLProgram*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_GLProgram_getProgram'\n", nullptr);
		return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    if (0 == argc)
    {
        unsigned const int tolua_ret = ( unsigned const int)  self->getProgram();
        tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
        return 1;
    }
    
    CCLOG("'getProgram' function of GLProgram wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'getProgram'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_GLProgram_setUniformLocationF32(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    GLProgram* self = nullptr;
    int location = 0;
    double f1 = 0.0;
    double f2 = 0.0;
    double f3 = 0.0;
    double f4 = 0.0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.GLProgram",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = (GLProgram*)  tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2d_GLProgram_getProgram'\n", nullptr);
		return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc >= 2 && argc <= 5)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S,2,0,&tolua_err) ||
            !tolua_isnumber(tolua_S,3,0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        
        location = (int)  tolua_tonumber(tolua_S,2,0);
        f1 = (float)  tolua_tonumber(tolua_S,3,0);
        
        if (2 == argc)
        {
            self->setUniformLocationWith1f(location,f1);
            return 0;
        }
        
        if (argc >= 3)
        {
#if COCOS2D_DEBUG >= 1
            if (!tolua_isnumber(tolua_S,4,0,&tolua_err))
                goto tolua_lerror;
#endif
            
            f2 = (float)  tolua_tonumber(tolua_S,4,0);
            if (3 == argc)
            {
                self->setUniformLocationWith2f(location, f1, f2);
                return 0;
            }
        }
        
        if (argc >= 4)
        {
#if COCOS2D_DEBUG >= 1
            if (!tolua_isnumber(tolua_S,5,0,&tolua_err))
                goto tolua_lerror;
#endif
            
            f3 = (float)  tolua_tonumber(tolua_S,5,0);
            if (4 == argc)
            {
                self->setUniformLocationWith3f(location, f1, f2, f3);
                return 0;
            }
        }
        
        if (argc == 5)
        {
#if COCOS2D_DEBUG >= 1
            if (!tolua_isnumber(tolua_S,6,0,&tolua_err))
                goto tolua_lerror;
#endif
            
            f4 = (float)  tolua_tonumber(tolua_S,6,0);
            if (5 == argc)
            {
                self->setUniformLocationWith4f(location, f1, f2, f3, f4);
                return 0;
            }

        }
    }
    
    CCLOG("'setUniformLocationF32' function of GLProgram wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setUniformLocationF32'.",&tolua_err);
    return 0;
#endif    
    
}

static void extendGLProgram(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.GLProgram");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_GLProgram_create );
        lua_rawset(tolua_S,-3);
        
        lua_pushstring(tolua_S,"getProgram");
        lua_pushcfunction(tolua_S,tolua_cocos2d_GLProgram_getProgram );
        lua_rawset(tolua_S,-3);
        
        lua_pushstring(tolua_S,"setUniformLocationF32");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_GLProgram_setUniformLocationF32 );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_Texture2D_setTexParameters(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    Texture2D* self = nullptr;
    GLuint arg1 = 0;
    GLuint arg2 = 0;
    GLuint arg3 = 0;
    GLuint arg4 = 0;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S,1,"cc.Texture2D",0,&tolua_err)) goto tolua_lerror;
#endif
    
    self = (Texture2D*)  tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (nullptr == self)
    {
        tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_Texture2D_setTexParameters'\n", nullptr);
		return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (4 == argc)
    {        
#if COCOS2D_DEBUG >= 1
        if (!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 4, 0, &tolua_err) ||
            !tolua_isnumber(tolua_S, 5, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        
        arg1 = (GLuint)tolua_tonumber(tolua_S, 2, 0);
        arg2 = (GLuint)tolua_tonumber(tolua_S, 3, 0);
        arg3 = (GLuint)tolua_tonumber(tolua_S, 4, 0);
        arg4 = (GLuint)tolua_tonumber(tolua_S, 5, 0);
        
        Texture2D::TexParams param = { arg1, arg2, arg3, arg4 };
        
        self->setTexParameters(param);
        
        return 0;
    }
    
    CCLOG("'setTexParameters' function of Texture2D wrong number of arguments: %d, was expecting %d\n", argc,4);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'setTexParameters'.",&tolua_err);
    return 0;
#endif
}

static int tolua_cocos2dx_SpriteBatchNode_getDescendants(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    cocos2d::SpriteBatchNode* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.SpriteBatchNode",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::SpriteBatchNode*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2dx_SpriteBatchNode_getDescendants'", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        if(!ok)
            return 0;
        std::vector<Sprite*> ret = cobj->getDescendants();

        lua_newtable(tolua_S);
        
        if (ret.empty())
            return 1;
        
        auto iter = ret.begin();
        int  indexTable = 1;
        for (; iter != ret.end(); ++iter)
        {
            if (nullptr == *iter)
                continue;
            
            lua_pushnumber(tolua_S, (lua_Number)indexTable);
            toluafix_pushusertype_ccobject(tolua_S, (*iter)->_ID, &((*iter)->_luaID), (void*)(*iter),"cc.Sprite");
            lua_rawset(tolua_S, -3);
            (*iter)->retain();
            ++indexTable;
        }
        
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getDescendants",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_SpriteBatchNode_getDescendants'.",&tolua_err);
#endif
    return 0;
}

static void extendTexture2D(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Texture2D");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setTexParameters");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_Texture2D_setTexParameters );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Node");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"scheduleUpdate");
        lua_pushcfunction(tolua_S,tolua_Cocos2d_Node_scheduleUpdate);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S,"unscheduleUpdate");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Node_unscheduleUpdate);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S,"getPosition");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Node_getPosition);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setContentSize");
        lua_pushcfunction(tolua_S, tolua_cocos2d_Node_setContentSize);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setAnchorPoint");
        lua_pushcfunction(tolua_S, tolua_cocos2d_Node_setAnchorPoint);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "addNodeEventListener");
        lua_pushcfunction(tolua_S, tolua_cocos2d_Node_addNodeEventListener);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "removeNodeEventListener");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_removeNodeEventListener);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "removeNodeEventListenersByEvent");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_removeNodeEventListenersByEvent);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "removeNodeEventListenersByTag");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_removeNodeEventListenersByTag);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "removeAllNodeEventListeners");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_removeAllNodeEventListeners);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "hasNodeEventListener");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_hasNodeEventListener);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "isTouchEnabled");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_isTouchEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setTouchEnabled");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_setTouchEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "isTouchSwallowEnabled");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_isTouchSwallowEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setTouchSwallowEnabled");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_setTouchSwallowEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "isTouchCaptureEnabled");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_isTouchCaptureEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setTouchCaptureEnabled");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_setTouchCaptureEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setTouchMode");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_setTouchMode);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "getTouchMode");
        lua_pushcfunction(tolua_S, tolua_Cocos2d_Node_getTouchMode);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "setKeyboardEnabled");
        lua_pushcfunction(tolua_S, lua_cocos2dx_Node_setKeyboardEnabled);
        lua_rawset(tolua_S, -3);
        lua_pushstring(tolua_S, "isKeyboardEnabled");
        lua_pushcfunction(tolua_S, lua_cocos2dx_Node_isKeyboardEnabled);
        lua_rawset(tolua_S, -3);
    }
    lua_pop(tolua_S, 1);
}

//static void extendLayer(lua_State* tolua_S)
//{
//    lua_pushstring(tolua_S,"cc.Layer");
//    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
//    if (lua_istable(tolua_S,-1))
//    {
//        lua_pushstring(tolua_S, "registerScriptKeypadHandler");
//        lua_pushcfunction(tolua_S, tolua_cocos2d_Layer_registerScriptKeypadHandler);
//        lua_rawset(tolua_S, -3);
//        lua_pushstring(tolua_S, "unregisterScriptKeypadHandler");
//        lua_pushcfunction(tolua_S, tolua_cocos2d_Layer_unregisterScriptKeypadHandler);
//        lua_rawset(tolua_S, -3);
//        lua_pushstring(tolua_S, "registerScriptAccelerateHandler");
//        lua_pushcfunction(tolua_S, tolua_cocos2d_Layer_registerScriptAccelerateHandler);
//        lua_rawset(tolua_S, -3);
//        lua_pushstring(tolua_S, "unregisterScriptAccelerateHandler");
//        lua_pushcfunction(tolua_S, tolua_cocos2d_Layer_unregisterScriptAccelerateHandler);
//        lua_rawset(tolua_S, -3);
//        
//        tolua_function(tolua_S, "setKeyboardEnabled", lua_cocos2dx_Layer_setKeyboardEnabled);
//        tolua_function(tolua_S, "isKeyboardEnabled", lua_cocos2dx_Layer_isKeyboardEnabled);
//        tolua_function(tolua_S, "setAccelerometerEnabled", lua_cocos2dx_Layer_setAccelerometerEnabled);
//        tolua_function(tolua_S, "isAccelerometerEnabled", lua_cocos2dx_Layer_isAccelerometerEnabled);
//        tolua_function(tolua_S, "setAccelerometerInterval", lua_cocos2dx_Layer_setAccelerometerInterval);
//    }
//    lua_pop(tolua_S, 1);
//}

static void extendScheduler(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Scheduler");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"scheduleScriptFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Scheduler_scheduleScriptFunc);
        lua_rawset(tolua_S,-3);
        lua_pushstring(tolua_S, "unscheduleScriptEntry");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Scheduler_unscheduleScriptEntry);
        lua_rawset(tolua_S, -3);
    }
    lua_pop(tolua_S, 1);
}

static void extendSequence(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Sequence");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Sequence_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCallFunc(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CallFunc");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CallFunc_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendSpawn(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Spawn");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_Spawn_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCardinalSplineBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CardinalSplineBy");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,lua_cocos2d_CardinalSplineBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCatmullRomBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CatmullRomBy");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CatmullRomBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendCatmullRomTo(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.CatmullRomTo");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_CatmullRomTo_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendBezierBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.BezierBy");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_BezierBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendBezierTo(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.BezierTo");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2d_BezierTo_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendDrawNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.DrawNode");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"drawPolygon");
        lua_pushcfunction(tolua_S,tolua_cocos2d_DrawNode_drawPolygon);
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_DrawNode_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendSprite(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.Sprite");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_Sprite_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendLayerColor(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.LayerColor");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_LayerColor_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendLayerMultiplex(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.LayerMultiplex");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_LayerMultiplex_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendParticleSystem(lua_State* tolua_S)
{
    lua_pushstring(tolua_S,"cc.ParticleSystem");
    lua_rawget(tolua_S,LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_ParticleSystem_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendUserDefault(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.UserDefault");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"getInstance");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_UserDefault_getInstance );
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendSpriteBatchNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.SpriteBatchNode");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"getDescendants");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_SpriteBatchNode_getDescendants );
        lua_rawset(tolua_S,-3);

        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_SpriteBatchNode_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendMotionStreak(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.MotionStreak");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_MotionStreak_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendAtlasNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.AtlasNode");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_AtlasNode_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static void extendParticleBatchNode(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.ParticleBatchNode");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        lua_pushstring(tolua_S,"setBlendFunc");
        lua_pushcfunction(tolua_S,tolua_cocos2dx_ParticleBatchNode_setBlendFunc);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

NS_CC_BEGIN
EventListenerAcceleration* LuaEventListenerAcceleration::create()
{
    EventListenerAcceleration* eventAcceleration = new EventListenerAcceleration();
    if (nullptr == eventAcceleration)
        return nullptr;
    
    if ( eventAcceleration->init([=](Acceleration* acc, Event* event){
        LuaEventAccelerationData listenerData((void*)acc,event);
        BasicScriptData data(eventAcceleration,(void*)&listenerData);
        LuaEngine::getInstance()->handleEvent(ScriptHandlerMgr::HandlerType::EVENT_ACC, (void*)&data);
    }))
    {
        eventAcceleration->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(eventAcceleration);
    }
    return eventAcceleration;
}

EventListenerCustom* LuaEventListenerCustom::create(const std::string& eventName)
{
    EventListenerCustom* eventCustom = new EventListenerCustom();
    if (nullptr == eventCustom)
        return nullptr;
    
    if ( eventCustom->init(eventName, [=](EventCustom* event){
        BasicScriptData data((void*)eventCustom,(void*)event);
        LuaEngine::getInstance()->handleEvent(ScriptHandlerMgr::HandlerType::EVENT_CUSTIOM, (void*)&data );
    }))
    {
        eventCustom->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(eventCustom);
    }
    return eventCustom;
}
NS_CC_END

static int tolua_cocos2d_LuaEventListenerCustom_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerCustom", 0, &tolua_err))  goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!tolua_isstring(tolua_S,2,0,&tolua_err) ||
            !toluafix_isfunction(tolua_S,3,"LUA_FUNCTION",0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        const std::string eventName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,3,0);
        cocos2d::EventListenerCustom* tolua_ret = LuaEventListenerCustom::create(eventName);
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)tolua_ret, handler, ScriptHandlerMgr::HandlerType::EVENT_CUSTIOM);
        
        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerCustom");
        
        return 1;
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerCustom(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerCustom");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create",tolua_cocos2d_LuaEventListenerCustom_create);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_EventListenerMouse_create(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S, 1, "cc.EventListenerMouse", 0, &tolua_err))  goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 1)
    {
        cocos2d::EventListenerMouse* tolua_ret = cocos2d::EventListenerMouse::create();
        if(nullptr == tolua_ret)
            return 0;

        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerMouse");
        
        return 1;
    }
    
    CCLOG("'create' has wrong number of arguments: %d, was expecting %d\n", argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'create'.",&tolua_err);
    return 0;
#endif
}

static void cloneMouseHandler(const EventListenerMouse* src,EventListenerMouse* dst,  ScriptHandlerMgr::HandlerType type)
{
    if (nullptr == src || nullptr == dst)
        return;
    
    LUA_FUNCTION handler = ScriptHandlerMgr::getInstance()->getObjectHandler((void*)src, type);
    if (0 != handler)
    {
        int newscriptHandler = cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->reallocateScriptHandler(handler);
        
        ScriptHandlerMgr::getInstance()->addObjectHandler((void*)dst, newscriptHandler, type);
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN:
                {
                    dst->onMouseDown = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP:
                {
                    dst->onMouseUp = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE:
                {
                    dst->onMouseMove = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL:
                {
                    dst->onMouseScroll = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)dst,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            default:
                break;
        }
    }
}

static int tolua_cocos2dx_EventListenerMouse_clone(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    EventListenerMouse* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerMouse", 0, &tolua_err))  goto tolua_lerror;
#endif
    
    self = static_cast<EventListenerMouse*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerMouse_clone'\n", nullptr);
		return 0;
	}
#endif
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 0)
    {
        cocos2d::EventListenerMouse* tolua_ret = cocos2d::EventListenerMouse::create();
        if(nullptr == tolua_ret)
            return 0;
        
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN);
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE);
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL);
        cloneMouseHandler(self, tolua_ret, ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP);
        
        int ID = (tolua_ret) ? (int)tolua_ret->_ID : -1;
        int* luaID = (tolua_ret) ? &tolua_ret->_luaID : NULL;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)tolua_ret,"cc.EventListenerMouse");
        
        return 1;
    }
    
    CCLOG("'clone' has wrong number of arguments: %d, was expecting %d\n", argc, 0);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'clone'.",&tolua_err);
    return 0;
#endif
}


static int tolua_cocos2dx_EventListenerMouse_registerScriptHandler(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    EventListenerMouse* self = nullptr;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertype(tolua_S, 1, "cc.EventListenerMouse", 0, &tolua_err))  goto tolua_lerror;
#endif
    
    self = static_cast<EventListenerMouse*>(tolua_tousertype(tolua_S,1,0));
#if COCOS2D_DEBUG >= 1
    if (nullptr == self) {
		tolua_error(tolua_S,"invalid 'self' in function 'tolua_cocos2dx_EventListenerMouse_registerScriptHandler'\n", nullptr);
		return 0;
	}
#endif
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc == 2)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S,2,"LUA_FUNCTION",0,&tolua_err) ||
            !tolua_isnumber(tolua_S, 3, 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function(tolua_S,2,0);
        ScriptHandlerMgr::HandlerType type = static_cast<ScriptHandlerMgr::HandlerType>((int)tolua_tonumber(tolua_S, 3, 0) ) ;
        
        switch (type)
        {
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_DOWN:
                {
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                
                    self->onMouseDown = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_UP:
                {
                    self->onMouseUp = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_MOVE:
                {
                    self->onMouseMove = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            case ScriptHandlerMgr::HandlerType::EVENT_MOUSE_SCROLL:
                {
                    self->onMouseScroll = [=](Event* event){
                        LuaEventMouseData mouseData(event);
                        BasicScriptData data((void*)self,(void*)&mouseData);
                        LuaEngine::getInstance()->handleEvent(type, (void*)&data);
                    };
                
                    ScriptHandlerMgr::getInstance()->addObjectHandler((void*)self, handler, type);
                }
                break;
            default:
                break;
        }
        return 0;
    }
    
    CCLOG("'registerScriptHandler' has wrong number of arguments: %d, was expecting %d\n", argc, 2);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'registerScriptHandler'.",&tolua_err);
    return 0;
#endif
}

static void extendEventListenerMouse(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.EventListenerMouse");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "create", tolua_cocos2dx_EventListenerMouse_create);
        tolua_function(tolua_S, "registerScriptHandler", tolua_cocos2dx_EventListenerMouse_registerScriptHandler);
        tolua_function(tolua_S, "clone", tolua_cocos2dx_EventListenerMouse_clone);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_ActionCamera_reverse(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    cocos2d::ActionCamera* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.ActionCamera",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::ActionCamera*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2dx_ActionCamera_reverse'", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        cocos2d::ActionCamera* reverse = cobj->reverse();
        toluafix_pushusertype_ccobject(tolua_S, reverse->_ID, &(reverse->_luaID), (void*)(reverse),"cc.ActionCamera");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "reverse",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_ActionCamera_reverse'.",&tolua_err);
#endif
    return 0;
}

static void extendActionCamera(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.ActionCamera");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "reverse", tolua_cocos2dx_ActionCamera_reverse);
    }
    lua_pop(tolua_S, 1);
}

static int tolua_cocos2dx_GridAction_reverse(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    int argc = 0;
    cocos2d::GridAction* cobj = nullptr;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.GridAction",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::GridAction*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_cocos2dx_GridAction_reverse'", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 0)
    {
        cocos2d::GridAction* reverse = cobj->reverse();
        toluafix_pushusertype_ccobject(tolua_S, reverse->_ID, &(reverse->_luaID), (void*)(reverse),"cc.GridAction");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "reverse",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'tolua_cocos2dx_GridAction_reverse'.",&tolua_err);
#endif
    return 0;
}

static void extendGridAction(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.GridAction");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "reverse", tolua_cocos2dx_GridAction_reverse);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_Label_createWithTTF00(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    int argc = 0;
    bool ok = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"cc.Label",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(L) - 1;
    
    if (argc >= 2 && argc <= 4)
    {
        
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3, 0, &tolua_err) ||
            !tolua_isnumber(L, 4, 1, &tolua_err) ||
            !tolua_isnumber(L, 5, 1, &tolua_err) )
        {
            goto tolua_lerror;
        }
#endif
        TTFConfig ttfConfig("");
        std::string text = "";

        ok &= luaval_to_ttfconfig(L, 2, &ttfConfig);
        if (!ok)
            return 0;
        
        ok &= luaval_to_std_string(L, 3, &text);
        if (!ok)
            return 0;
        
        
        int alignment = tolua_tonumber(L, 4, 1);
        int lineSize  = tolua_tonumber(L, 5, 0);
        cocos2d::Label* ret = cocos2d::Label::createWithTTF(ttfConfig, text, static_cast<TextHAlignment>(alignment), lineSize);
        int ID = ret ? (int)(ret->_ID) : -1;
        int* luaID = ret ? &(ret->_luaID) : nullptr;
        toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,"cc.Label");
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(L,"#ferror in function 'lua_cocos2dx_Label_createWithTTF'.",&tolua_err);
#endif
    return 0;
}

static int lua_cocos2dx_Label_createWithTTF01(lua_State* L)
{
    if (nullptr == L)
        return 0;
    
    int argc = 0;
    
    tolua_Error tolua_err;
    if (!tolua_isusertable(L,1,"cc.Label",0,&tolua_err)) goto tolua_lerror;
    
    argc = lua_gettop(L) - 1;
    
    if (argc >= 3 && argc <= 6)
    {
        if (!tolua_isstring(L, 2, 0, &tolua_err)  ||
            !tolua_isstring(L, 3, 0, &tolua_err)  ||
            !tolua_isnumber(L, 4, 0, &tolua_err)  ||
            !tolua_istable(L, 5, 1, &tolua_err)   ||
            !tolua_isnumber(L, 6, 1, &tolua_err)  ||
            !tolua_isnumber(L, 7, 1, &tolua_err) )
        {
            goto tolua_lerror;
        }
        else
        {
            std::string text = tolua_tostring(L, 2, "");
            std::string fontFile = tolua_tostring(L, 3, "");
            float fontSize   = tolua_tonumber(L, 4, 0);
            cocos2d::Size dimensions = cocos2d::Size::ZERO;
            if (lua_istable(L, 5))
            {
                luaval_to_size(L, 5, &dimensions);
            }
            TextHAlignment hAlignment = static_cast<TextHAlignment>((int)tolua_tonumber(L, 6, 0));
            TextVAlignment vAlignment = static_cast<TextVAlignment>((int)tolua_tonumber(L, 7, 0));
            
            cocos2d::Label* ret = cocos2d::Label::createWithTTF(text, fontFile, fontSize, dimensions, hAlignment, vAlignment);
            
            int ID = ret ? (int)(ret->_ID) : -1;
            int* luaID = ret ? &(ret->_luaID) : nullptr;
            toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,"cc.Label");
            return 1;
        }
    }
    
tolua_lerror:
    return lua_cocos2dx_Label_createWithTTF00(L);
}

extern int lua_cocos2dx_Label_create(lua_State* tolua_S);

static int lua_cocos2dx_Label_create_deprecated(lua_State* tolua_S)
{
    if (nullptr == tolua_S)
        return 0;
    
    int argc = 0;
    
    tolua_Error tolua_err;
    if (!tolua_isusertable(tolua_S,1,"cc.Label",0,&tolua_err)) goto tolua_lerror;
    
    argc = lua_gettop(tolua_S) - 1;
    
    if (argc >= 3 && argc <= 6)
    {
        if (!tolua_isstring(tolua_S, 2, 0, &tolua_err)  ||
            !tolua_isstring(tolua_S, 3, 0, &tolua_err)  ||
            !tolua_isnumber(tolua_S, 4, 0, &tolua_err)  ||
            !tolua_istable(tolua_S, 5, 1, &tolua_err)   ||
            !tolua_isnumber(tolua_S, 6, 1, &tolua_err)  ||
            !tolua_isnumber(tolua_S, 7, 1, &tolua_err) )
        {
            goto tolua_lerror;
        }
        else
        {
            CCLOG("The create(text, ...) was deprecated,please use create createWithTTF(text, ...) instead");
            std::string text = tolua_tocppstring(tolua_S, 2, "");
            std::string fontFile = tolua_tocppstring(tolua_S, 3, "");
            float fontSize   = tolua_tonumber(tolua_S, 4, 0);
            cocos2d::Size dimensions = cocos2d::Size::ZERO;
            if (lua_istable(tolua_S, 5))
            {
                luaval_to_size(tolua_S, 5, &dimensions);
            }
            TextHAlignment hAlignment = static_cast<TextHAlignment>((int)tolua_tonumber(tolua_S, 6, 0));
            TextVAlignment vAlignment = static_cast<TextVAlignment>((int)tolua_tonumber(tolua_S, 7, 0));
            
            cocos2d::Label* ret = cocos2d::Label::create(text, fontFile, fontSize, dimensions, hAlignment, vAlignment);
            
            int ID = ret ? (int)(ret->_ID) : -1;
            int* luaID = ret ? &(ret->_luaID) : nullptr;
            toluafix_pushusertype_ccobject(tolua_S,ID, luaID, (void*)ret,"cc.Label");
            return 1;
        }
    }
    
tolua_lerror:
    return lua_cocos2dx_Label_create(tolua_S);
}

static void extendLabel(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Label");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "createWithTTF", lua_cocos2dx_Label_createWithTTF00);
        tolua_function(tolua_S, "createWithTTF", lua_cocos2dx_Label_createWithTTF01);
        tolua_function(tolua_S, "create", lua_cocos2dx_Label_create_deprecated);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_TMXTiledMap_getPropertiesForGID(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::TMXTiledMap* cobj = NULL;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.TMXTiledMap",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::TMXTiledMap*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_TMXTiledMap_getPropertiesForGID'", NULL);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
            
        if (!ok)
            return 0;
        cocos2d::Value ret = cobj->getPropertiesForGID(arg0);
        ccvalue_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getPropertiesForGID",argc, 1);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_TMXTiledMap_getPropertiesForGID'.",&tolua_err);
#endif
    
    return 0;
}

static void extendTMXTiledMap(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.TMXTiledMap");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S, "getPropertiesForGID", lua_cocos2dx_TMXTiledMap_getPropertiesForGID);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_Console_send(lua_State* tolua_S)
{
    cocos2d::Console* cobj = nullptr;
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Console",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Console*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Console_send'", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    
    if (argc == 2)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,&arg0);
        std::string arg1;
        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        
        send(arg0, arg1.c_str(), arg1.length(), 0);
        return 0;
    }
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "send",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Console_send'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_Console_wait(lua_State* tolua_S)
{
    cocos2d::Console* cobj = nullptr;
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Console",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (cocos2d::Console*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Console_wait'", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    
    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        
        std::chrono::milliseconds dura( arg0 * 1000 );
        std::this_thread::sleep_for( dura );
        return 0;
    }
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "wait",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Console_wait'.",&tolua_err);
#endif
    
    return 0;
}

static int lua_cocos2dx_Console_addCommand(lua_State* tolua_S)
{
    cocos2d::Console* cobj = nullptr;
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.Console",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::Console*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_Console_addCommand'", NULL);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (ok && argc == 2)
    {
        ValueMap arg0;
        ok &= luaval_to_ccvaluemap(tolua_S, 2, &arg0);
        //
        std::string name = std::string(arg0["name"].asString());
        std::string help = std::string(arg0["help"].asString());
        
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = 0;
        if (ok) {
            handler = (  toluafix_ref_function(tolua_S,3,0));
            ScriptHandlerMgr::getInstance()->addCustomHandler((void*)cobj, handler);
            
            struct Console::Command outValue = {
                name,
                help,
                [=](int fd, const std::string& args)
                {
                    //lua-callback, the third param;
                    tolua_pushnumber(tolua_S, fd);
                    tolua_pushstring(tolua_S, args.c_str());
                    
                    LuaEngine::getInstance()->getLuaStack()->executeFunctionByHandler(handler, 2);
                }
            };
            cobj->addCommand(outValue);
        }
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "addCommand",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_Console_addCommand'.",&tolua_err);
#endif
    
    return 0;
}

static void extendConsole(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.Console");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S,"wait", lua_cocos2dx_Console_wait);
        tolua_function(tolua_S,"send", lua_cocos2dx_Console_send);
        tolua_function(tolua_S,"addCommand", lua_cocos2dx_Console_addCommand);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_GLProgramState_setVertexAttribPointer(lua_State* tolua_S)
{
    int argc = 0;
    cocos2d::GLProgramState* cobj = nullptr;
    bool ok  = true;
    
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"cc.GLProgramState",0,&tolua_err)) goto tolua_lerror;
#endif
    
    cobj = (cocos2d::GLProgramState*)tolua_tousertype(tolua_S,1,0);
    
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_GLProgramState_setVertexAttribPointer'", nullptr);
        return 0;
    }
#endif
    
    argc = lua_gettop(tolua_S)-1;
    if (argc == 6)
    {
        std::string arg0;
        int arg1;
        unsigned int arg2;
        bool arg3;
        int arg4;
        int arg5;
        
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        
        ok &= luaval_to_uint32(tolua_S, 4,&arg2);
        
        ok &= luaval_to_boolean(tolua_S, 5, &arg3);
        
        ok &= luaval_to_int32(tolua_S, 6,(int *)&arg4);
        
        ok &= luaval_to_int32(tolua_S, 7, (int *)&arg5);
        
        if(!ok)
            return 0;
        cobj->setVertexAttribPointer(arg0, arg1, arg2, arg3, arg4, (void*)arg5);
        return 0;
    }
    else if (argc == 7)
    {
        std::string arg0;
        int arg1;
        unsigned int arg2;
        bool arg3;
        int arg4;
        GLfloat* arg5;
        int arg6;
        
        
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        
        ok &= luaval_to_uint32(tolua_S, 4,&arg2);
        
        ok &= luaval_to_boolean(tolua_S, 5, &arg3);
        
        ok &= luaval_to_int32(tolua_S, 6,(int *)&arg4);
        
        ok &= luaval_to_int32(tolua_S, 8, (int *)&arg6);
        
        size_t len = lua_objlen(tolua_S, 7);
        
        if (len != arg6)
        {
            CCLOG("table size is  %zu,but input size is %d \n", len, arg6);
            return 0;
        }
        
        arg5 = new GLfloat[len];
        for (int i = 0; i < len; i++)
        {
            lua_pushnumber(tolua_S,i + 1);
            lua_gettable(tolua_S,7);
            bool isnum = true;
#if COCOS2D_DEBUG >= 1
            if (!tolua_isnumber(tolua_S, -1, 0, &tolua_err))
            {
                isnum = false;
            }
#endif
            if (isnum)
            {
                arg5[i] = tolua_tonumber(tolua_S, -1, 0);
            }
            else
            {
                arg5[i] = 0;
            }
            lua_pop(tolua_S, 1);
        }
        
        cobj->setVertexAttribPointer(arg0, arg1, arg2, arg3, arg4, (void*)arg5);
        
        CC_SAFE_DELETE(arg5);
        
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setVertexAttribPointer",argc, 6);
    return 0;
    
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_GLProgramState_setVertexAttribPointer'.",&tolua_err);
#endif
    
    return 0;
}

static void extendGLProgramState(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.GLProgramState");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S,-1))
    {
        tolua_function(tolua_S,"setVertexAttribPointer", lua_cocos2dx_GLProgramState_setVertexAttribPointer);
    }
    lua_pop(tolua_S, 1);
}

int lua_cocos2dx_RotateBy_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif
    
#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"cc.RotateBy",0,&tolua_err)) goto tolua_lerror;
#endif
    
    argc = lua_gettop(tolua_S)-1;
    
    do
    {
        if (argc == 3)
        {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);
            if (!ok) { break; }
            double arg1;
            ok &= luaval_to_number(tolua_S, 3,&arg1);
            if (!ok) { break; }
            double arg2;
            ok &= luaval_to_number(tolua_S, 4,&arg2);
            if (!ok) { break; }
            cocos2d::RotateBy* ret = cocos2d::RotateBy::create(arg0, arg1, arg2);
            object_to_luaval<cocos2d::RotateBy>(tolua_S, "cc.RotateBy",(cocos2d::RotateBy*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do
    {
        tolua_Error tolua_err;
        if (!tolua_isnumber(tolua_S,3,0,&tolua_err))
        {
            break;
        }
        if (argc == 2)
        {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);
            if (!ok) { break; }
            double arg1;
            ok &= luaval_to_number(tolua_S, 3,&arg1);
            if (!ok) { break; }
            cocos2d::RotateBy* ret = cocos2d::RotateBy::create(arg0, arg1);
            object_to_luaval<cocos2d::RotateBy>(tolua_S, "cc.RotateBy",(cocos2d::RotateBy*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do
    {
        if (argc == 2)
        {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);
            if (!ok) { break; }
            cocos2d::Vec3 arg1;
            ok &= luaval_to_vec3(tolua_S, 3, &arg1);
            if (!ok) { break; }
            cocos2d::RotateBy* ret = cocos2d::RotateBy::create(arg0, arg1);
            object_to_luaval<cocos2d::RotateBy>(tolua_S, "cc.RotateBy",(cocos2d::RotateBy*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_RotateBy_create'.",&tolua_err);
#endif
    return 0;
}

static void extendRotateBy(lua_State* tolua_S)
{
    lua_pushstring(tolua_S, "cc.RotateBy");
    lua_rawget(tolua_S, LUA_REGISTRYINDEX);
    if (lua_istable(tolua_S, -1))
    {
        lua_pushstring(tolua_S,"create");
        lua_pushcfunction(tolua_S,lua_cocos2dx_RotateBy_create);
        lua_rawset(tolua_S,-3);
    }
    lua_pop(tolua_S, 1);
}

static int lua_cocos2dx_cocos2dVersion(lua_State* tolua_S)
{
    const char* ver = cocos2dVersion();
    lua_pushstring(tolua_S, ver);
    return 1;
}

static void extendFunctions(lua_State* tolua_S)
{
    tolua_module(tolua_S,"cc",0);
    tolua_beginmodule(tolua_S,"cc");
        tolua_function(tolua_S,"LuaLoadChunksFromZIP",LuaStack::lua_loadChunksFromZIP);
        tolua_function(tolua_S,"cocos2dVersion",lua_cocos2dx_cocos2dVersion);
    tolua_endmodule(tolua_S);
}

static void extendConstants(lua_State* tolua_S)
{
    tolua_module(tolua_S,"cc",0);
    tolua_beginmodule(tolua_S,"cc");
        lua_pushstring(tolua_S,"EVENT_COME_TO_FOREGROUND");
        lua_pushstring(tolua_S,EVENT_COME_TO_FOREGROUND);
        lua_rawset(tolua_S,-3);
        lua_pushstring(tolua_S,"EVENT_RENDERER_RECREATED");
        lua_pushstring(tolua_S,EVENT_RENDERER_RECREATED);
        lua_rawset(tolua_S,-3);
        lua_pushstring(tolua_S,"EVENT_COME_TO_BACKGROUND");
        lua_pushstring(tolua_S,EVENT_COME_TO_BACKGROUND);
        lua_rawset(tolua_S,-3);
    tolua_endmodule(tolua_S);
}

int register_all_cocos2dx_manual(lua_State* tolua_S)
{
    if (NULL == tolua_S)
        return 0;
    
    extendFunctions(tolua_S);
    extendConstants(tolua_S);
    extendNode(tolua_S);
//    extendLayer(tolua_S);
    extendScheduler(tolua_S);
    extendSequence(tolua_S);
    extendCallFunc(tolua_S);
    extendSpawn(tolua_S);
    extendCardinalSplineBy(tolua_S);
    extendCatmullRomBy(tolua_S);
    extendCatmullRomTo(tolua_S);
    extendBezierBy(tolua_S);
    extendBezierTo(tolua_S);
    extendDrawNode(tolua_S);
    extendSprite(tolua_S);
    extendLayerColor(tolua_S);
    extendLayerMultiplex(tolua_S);
    extendParticleSystem(tolua_S);
    extendUserDefault(tolua_S);
    extendGLProgram(tolua_S);
    extendTexture2D(tolua_S);
    extendSpriteBatchNode(tolua_S);
    extendEventListenerMouse(tolua_S);
    extendEventListenerCustom(tolua_S);
    extendActionCamera(tolua_S);
    extendGridAction(tolua_S);
    extendRotateBy(tolua_S);
    
    extendMotionStreak(tolua_S);
    extendAtlasNode(tolua_S);
    extendParticleBatchNode(tolua_S);
    extendLabel(tolua_S);
    extendTMXTiledMap(tolua_S);
    extendConsole(tolua_S);
    extendGLProgramState(tolua_S);
    
    return 0;
}