/****************************************************************************
 * Copyright (C) 2012 Woboq UG (haftungsbeschraenkt)
 * Olivier Goffart <contact at woboq.com>
 * http://woboq.com/
 ****************************************************************************/


#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

namespace clang {
class CXXMethodDecl;
class CXXRecordDecl;
class CXXConstructorDecl;
class EnumDecl;
class Preprocessor;
class Sema;
}


struct PropertyDef {
    std::string name, type, member, read, write, reset, designable, scriptable, editable, stored,
    user, notify, inPrivateClass;
    int notifyId = -1;
    bool constant = false;
    bool final = false;

    bool isEnum = false;

    int revision = 0;
};


struct ClassDef {

    clang::CXXRecordDecl *Record = nullptr;

    // This list only includes the things registered with the keywords
    std::vector<clang::CXXMethodDecl*> Signals;
    std::vector<clang::CXXMethodDecl*> Slots;
    std::vector<clang::CXXMethodDecl*> Methods;
    std::vector<clang::CXXConstructorDecl*> Constructors;
    std::vector<std::tuple<clang::EnumDecl*, std::string, bool>> Enums;

    void addEnum(clang::EnumDecl *E, std::string Alias, bool IsFlag) {
        for (auto I : Enums)
            if (std::get<1>(I) == Alias)
                return;

        Enums.emplace_back(E, std::move(Alias), IsFlag);
    }

    std::vector<clang::CXXRecordDecl *> Extra;
    void addExtra(clang::CXXRecordDecl *E) {
        if (!E || E == Record)
            return;
        if (std::find(Extra.begin(), Extra.end(), E) != Extra.end())
            return;
        Extra.push_back(E);
    }

    std::vector<std::pair<std::string, std::string>> Interfaces;
    std::vector<std::pair<std::string, std::string>> ClassInfo;

    std::vector<PropertyDef> Properties;

    bool HasQObject = false;
    bool HasQGadget = false;

    //TODO: PluginData;
    //TODO: FLagAliases;
};



ClassDef parseClass (clang::CXXRecordDecl* RD, clang::Sema& Sema);
