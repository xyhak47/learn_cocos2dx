#####################
##   code by xyh   ##
#####################

import sys, os, stat
import argparse

##############################################################################################################################
template_service_h_usual = '''
//
// crated by auto-script generation
//
#ifndef __BotDota__{class}Service__
#define __BotDota__{class}Service__

#include "cocos2d.h"
#include "gc.h"
#include "Services/BaseService.h"

class {class}Service : public BaseService
{{
    GC_SINGLETON_WITH_DEFAULT_CONSTRUCTOR({class}Service);
    
public:
    static Ref* getData(std::string msgKey);
'''

template_service_h_reuse = '''
public:
    static void {funcName}();
private:
    void request{funcName_upper}(); 
    void response{funcName_upper}(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response);
'''

template_service_h_tail = '''
};

#endif
'''
###################################################################################################################################################
template_service_cpp_usual = '''
#include "Services/{class}Service.h"
#include "utils/gcstring.h"
#include "Services/PlayerCache.h"

Ref* {class}Service::getData(std::string msgKey)
{{
    return {class}Service::shared{class}Service()->getStateData(msgKey);
}}



'''

template_service_cpp_reuse = '''
void {class}Service::{funcName}()
{{
    {class}Service::shared{class}Service()->request{funcName_upper}();
}}

void {class}Service::request{funcName_upper}()
{{
    std::map<std::string, std::string> params;
    
    const std::string &playerName(PlayerCache::sharedPlayerCache()->getPlayerName());
    
    params.insert(std::make_pair("userName", playerName));
   
    std::string url(gc::net::getFullUrl("{class_lower}_{funcName}", params));
    gc::net::sendRequest(url, this, httpresponse_selector({class}Service::response{funcName_upper}));
}}

void {class}Service::response{funcName_upper}(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response)
{{
    NetMessageInfo *message = gc::net::resolveData(response);
    if (validateNetMessage(message))
    {{
        if (kNetDataCsv == message->getDataType())
        {{
            if (message->getCsvData().empty())
            {{
                return;
            }}
        }}
    }}
}}




'''
###################################################################################################################################################
template_info_h_usual = '''
//
// crated by auto-script generation
//
#ifndef __BotDota__{class}Info__
#define __BotDota__{class}Info__

#include "cocos2d.h"
#include "base/gcMacros.h"
#include "net/entities/DataTableInfo.h"
#include "base/ArrProxy.h"

using std::string;

class {class}Info : public cocos2d::Ref
{{
public:
    GC_CREATE_FUNC({class}Info);
protected:
    {class}Info();
    
public:
    static gc::ArrProxy* create{class}InfoArrayCsv(const DataTableInfo &table);
private:
    static {class}Info* create{class}InfoCsv(const DataTableInfo &table, int rowIndex);
    
private:
    void setValueByCsv(const DataTableInfo &table, int rowIndex);
'''
    
template_info_h_reuse = '''
    GC_SYNTHESIZE_READONLY({propertyTpye}, m_{property},  {PropertyUpper});'''
template_info_h_reuse_intArray = '''
public:
    std::string get{PropertyUpper}();
private:
    std::vector<int> m_{property};'''
template_info_h_reuse_stringArray = '''
public:
    std::string get{PropertyUpper}();
private:
    std::vector<std::string> m_{property};'''
template_info_h_tail = '''
};
#endif
'''
###################################################################################################################################################
template_info_cpp_usual = '''
#include "Data/{class}Info.h"
#include "DataColumn/DataColumnName.h"
#include "utils/gcstring.h"

USING_NS_CC;
USING_NS_GC;

#define {class}_ResultDataOpe   DataOperat dataOpe[] = \\
{{ \\'''
template_info_cpp_reuse = '''
{{"{property}",  &m_{property},   set{PropertyUpper}MemberData,    format{PropertyUpper}MemberData, set{PropertyUpper}MemberData}},\\'''
template_info_cpp_reuse_tail = '''
}}
'''
template_info_cpp_ctor_head = '''
{class}Info::{class}Info()'''
template_info_cpp_ctor_body_first = '''
:m_{property}({initData})'''
template_info_cpp_ctor_body_rest = '''
,m_{property}({initData})'''
template_info_cpp_ctor_tail = '''
{{
}}
'''

template_info_cpp_body = '''
ArrProxy* {class}Info::create{class}InfoArrayCsv(const DataTableInfo &table)
{{
    int rowLength = table.getRowLength();
    ArrProxy *result = ArrProxy::createWithCapacity(rowLength);
    for (int i = 0; i < rowLength; ++i) {{
        {class}Info *item = create{class}InfoCsv(table, i);
        
        result->addObject(item);
    }}
    
    return result;
}}

{class}Info* {class}Info::create{class}InfoCsv(const DataTableInfo &table, int rowIndex)
{{
    {class}Info *item = {class}Info::create();
    item->setValueByCsv(table, rowIndex);
    
    return item;
}}

void {class}Info::setValueByCsv(const DataTableInfo &table, int rowIndex)
{{
    {class}_ResultDataOpe;
    int arrayLength(sizeof(dataOpe) / sizeof(*dataOpe));
    
    PARSE_CSV_TO_ENTITY(dataOpe, arrayLength, "{class}Info", table, rowIndex);
}}
'''
template_info_cpp_reuse_stringArray = '''
std::string {class}Info::get{PropertyUpper}()
{{
    return gc::utils::formatVecStringToString(m_{property}, ",");
}}
'''
template_info_cpp_reuse_intArray = '''
std::string {class}Info::get{PropertyUpper}()
{{
    return gc::utils::formatVecIntToString(m_{property}, ",");
}}
'''
##################################################################################################################################################
template_lua_parser_usual = '''
function parser._parse{class}Data()
    local data = __class__Service:getData(message.__MSG__)
    local dataArray = tolua.cast(data, "gc.ArrProxy")

    local itemsTable = {{}}

    for i=1,dataArray:count() do
        local obj = dataArray:objectAtIndex(i - 1)
        local item = tolua.cast(obj, "{class}Info")

        local itemInfo = {{}}
'''
template_lua_parser_reuse = '''
        itemInfo.{funcName} = item:get{funcName}()'''
template_lua_parser_reuse_array = '''
        itemInfo.{funcName} = tool.stringToTable(item:get{funcName}())'''
template_lua_parser_tail = '''
        table.insert(itemsTable, itemInfo)
    end

    return itemsTable
end
'''
##################################################################################################################################################
template_lua_service_usual = '''
function service.{funcName}(callBack)
    local function call(eventName)
        local items = parser.__PARSE__()

        callBack(eventName, items)
    end

    local function call_err(eventName)
        callBack(eventName, {{}})
    end

    listener.removeEvent(message.__MSG__)
    listener.addEvent(call, message.__MSG__)
    listener.addEvent(call_err, message.__MSG__)
    {class}Service:{funcName}()
end


'''
##################################################################################################################################################



def gen_service_h(out_path, classTable):
    name = classTable['class']
    funcs = classTable['funcs']

    t_usual = {}
    t_usual['class'] = name
    usual = template_service_h_usual.format(**t_usual)

    reuse = ''
    for func in funcs:
        t_reuse = {}
        t_reuse['funcName'] = func
        t_reuse['funcName_upper'] = func.capitalize()
        reuse += template_service_h_reuse.format(**t_reuse)

    tail = template_service_h_tail

    with open(out_path, 'w') as of:
        src = ''
        src = usual + reuse + tail
        of.write(src)
        of.close()




def gen_service_cpp(out_path, classTable):  
    name = classTable['class']
    funcs = classTable['funcs']

    t_usual = {}
    t_usual['class'] = name
    usual = template_service_cpp_usual.format(**t_usual)

    reuse = ''
    for func in funcs:
        t_reuse = {}
        t_reuse['class'] = name
        t_reuse['class_lower'] = name.lower()
        t_reuse['funcName'] = func
        t_reuse['funcName_upper'] = func.capitalize()
        reuse += template_service_cpp_reuse.format(**t_reuse)

    with open(out_path, 'w') as of:
        src = ''
        src = usual + reuse
        of.write(src)
        of.close()  




def gen_info_h(out_path, classTable):
    name = classTable['class']
    properties = classTable['properties']

    t_usual = {}
    t_usual['class'] = name
    usual = template_info_h_usual.format(**t_usual)

    reuse = ''
    for p in properties:
        temp = p.split("_")
        p_type = temp[0]
        p = temp[1]
        t_reuse = {}
        t_reuse['propertyTpye'] = p_type
        t_reuse['property'] = p
        t_reuse['PropertyUpper'] = p.capitalize()
        if p_type == 'intArray' or p_type == 'stringArray':
            continue
        reuse += template_info_h_reuse.format(**t_reuse)


    reuse_array = ''
    for p in properties:
        temp = p.split("_")
        p_type = temp[0]
        p = temp[1]
        t_reuse_array = {}
        t_reuse_array['property'] = p
        t_reuse_array['PropertyUpper'] = p.capitalize()
        if p_type == 'intArray':
            reuse_array += template_info_h_reuse_intArray.format(**t_reuse_array)
        elif p_type == 'stringArray':
            reuse_array += template_info_h_reuse_stringArray.format(**t_reuse_array)


    tail = template_info_h_tail

    with open(out_path, 'w') as of:
        src = ''
        src = usual + reuse + reuse_array + tail
        of.write(src)
        of.close()




def gen_info_cpp(out_path, classTable):
    name = classTable['class']
    properties = classTable['properties']

    t_usual = {}
    t_usual['class'] = name
    usual = template_info_cpp_usual.format(**t_usual)
    body = template_info_cpp_body.format(**t_usual)
    reuse_tail = template_info_cpp_reuse_tail.format(**t_usual)
    ctor_head = template_info_cpp_ctor_head.format(**t_usual)
    ctor_tail = template_info_cpp_ctor_tail.format(**t_usual)


    reuse = ''
    for p in properties:
        temp = p.split("_")
        p_type = temp[0]
        p = temp[1]
        t_reuse = {}
        t_reuse['property'] = p
        t_reuse['PropertyUpper'] = p_type[0].upper() + p_type[1:]
        reuse += template_info_cpp_reuse.format(**t_reuse)


    reuse_array = ''
    for p in properties:
        temp = p.split("_")
        p_type = temp[0]
        p = temp[1]
        t_reuse_array = {}
        t_reuse_array['class'] = name
        t_reuse_array['property'] = p
        t_reuse_array['PropertyUpper'] = p.capitalize()
        if p_type == 'intArray':
            reuse_array += template_info_cpp_reuse_intArray.format(**t_reuse_array)
        elif p_type == 'stringArray':
            reuse_array += template_info_cpp_reuse_stringArray.format(**t_reuse_array)


    ctor_body = ''
    for pro in properties:
        temp = pro.split("_")
        p_type = temp[0]
        p = temp[1]
        t_ctor_body_array = {}
        t_ctor_body_array['property'] = p
        t_ctor_body_array['initData'] = property_dir[p_type]
        if properties.index(pro) == 0:
            ctor_body += template_info_cpp_ctor_body_first.format(**t_ctor_body_array)
        else:
            ctor_body += template_info_cpp_ctor_body_rest.format(**t_ctor_body_array)


    with open(out_path, 'w') as of:
        src = ''
        src = usual + reuse + reuse_tail + ctor_head + ctor_body + ctor_tail + body + reuse_array
        of.write(src)
        of.close()




def gen_lua_parser(out_path, classTable):
    name = classTable['class']
    properties = classTable['properties']

    t_usual = {}
    t_usual['class'] = name
    usual = template_lua_parser_usual.format(**t_usual)

    reuse = ''
    for p in properties:
        temp = p.split("_")
        p_type = temp[0]
        p = temp[1]
        t_reuse = {}
        t_reuse['funcName'] = p.capitalize()
        if p_type == 'intArray' or p_type == 'stringArray':
            continue
        reuse += template_lua_parser_reuse.format(**t_reuse)


    reuse_array = ''
    for p in properties:
        temp = p.split("_")
        p_type = temp[0]
        p = temp[1]
        t_reuse_array = {}
        t_reuse_array['funcName'] = p.capitalize()
        if p_type == 'intArray' or p_type == 'stringArray':
            reuse_array += template_lua_parser_reuse_array.format(**t_reuse_array)


    tail = template_lua_parser_tail

    with open(out_path, 'a') as of:
        src = ''
        src = usual + reuse + reuse_array + tail
        of.write(src)
        of.close()




def gen_lua_service(out_path, classTable):
    name = classTable['class']
    funcs = classTable['funcs']

    usual = ''
    for func in funcs:
        t_usual = {}
        t_usual['class'] = name
        t_usual['funcName'] = func
        usual += template_lua_service_usual.format(**t_usual)

    with open(out_path, 'a') as of:
        of.write(usual)
        of.close()



def createServiceFile():
    for service in config_class_service:
        if service['update'] == 0:
            continue

        # create service h
        service_path_h = os.path.join(outdir_service, service['class'] + 'Service.h')
        gen_service_h(service_path_h, service)

        # create service cpp
        service_path_cpp = os.path.join(outdir_service, service['class'] + 'Service.cpp')
        gen_service_cpp(service_path_cpp, service)




def createInfoFile():
    for info in config_class_info:
        if info['update'] == 0:
            continue

        # create info h
        info_path_h = os.path.join(outdir_info, info['class'] + 'Info.h')
        gen_info_h(info_path_h, info)

        # create info cpp
        info_path_cpp = os.path.join(outdir_info, info['class'] + 'Info.cpp')
        gen_info_cpp(info_path_cpp, info)




def apppendLuaPaserFile():
    for info in config_class_info:
        if info['update'] == 0:
            continue

        lua_parser_path = os.path.join(outdir_lua_parser, 'parser.lua')
        gen_lua_parser(lua_parser_path, info)



def appendLuaSericeFile():
    for service in config_class_service:
        if service['update'] == 0:
            continue

        lua_service_path = os.path.join(outdir_lua_service, 'service.lua')
        gen_lua_service(lua_service_path, service)


def cur_file_dir():
    path = sys.path[0]
    if os.path.isdir(path):
        return path
    elif os.path.isfile(path):
        return os.path.dirname(path)



curdir = cur_file_dir()
outdir_service = os.path.join(curdir, '../../Classes/Services')
outdir_info = os.path.join(curdir, '../../Classes/Data')
outdir_lua_parser = os.path.join(curdir, '../../Resources/src/app/service')
outdir_lua_service = os.path.join(curdir, '../../Resources/src/app/service')



def main():
    createServiceFile()
    createInfoFile()
    apppendLuaPaserFile()
    appendLuaSericeFile()
	


def createServiceDic(updateflag, name, funcs):
    c = {}
    c['class'] = name
    c['funcs'] = funcs
    c['update'] = updateflag
    return c


def createInfoDic(updateflag, name, properties):
    c = {}
    c['class'] = name
    c['properties'] = properties
    c['update'] = updateflag
    return c



property_dir = {'int':0, 'string':'""', 'float':0.0, 'stringArray':'', 'intArray':''}


# warnning: set updateflag as 1 is to cover all the code in the file, usually set updateflag as 0
# How to use updateflag : set to 1 when debugging, Set to 0 after debugging.
# long -> use int
# updateflag, name, properties
config_class_info = [
    # demo,here by order : int_,string_,stringArray_,intArray_,
    createInfoDic(1, 'xixixix', ['string_nickName','int_requestTime','int_lv','float_fv','intArray_eeee','stringArray_yyyy']),
  
]




# updateflag, name, funcs
config_class_service = [
    # demo
    # createServiceDic(0, 'wfwfwf', ['addFriend', 'getFriendList', 'removeFriend', 'getFriendRequestList', 'giveGift', 'receiveGift']),
  
]






















if __name__ == '__main__':
	main()