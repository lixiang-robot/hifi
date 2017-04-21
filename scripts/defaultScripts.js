"use strict";
/* jslint vars: true, plusplus: true */

//
//  defaultScripts.js
//  examples
//
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//
var DEFAULT_SCRIPTS = [];

function pushAll(dest, orig) {
    for (var k in orig) { 
        dest.push(orig[k]);
    }
}

if (!App.isAndroid()) {
    pushAll(DEFAULT_SCRIPTS, [
        "system/progress.js",
        "system/away.js",
        "system/audio.js",
        "system/hmd.js",
        "system/menu.js",
        "system/bubble.js",
        "system/snapshot.js",
        "system/help.js",
        "system/pal.js", // "system/mod.js", // older UX, if you prefer
        "system/goto.js",
        "system/marketplaces/marketplaces.js",
        "system/edit.js",
        "system/tablet-users.js",
        "system/selectAudioDevice.js",
        "system/notifications.js",
        "system/controllers/controllerDisplayManager.js",
        "system/controllers/handControllerGrab.js",
        "system/controllers/handControllerPointer.js",
        "system/controllers/squeezeHands.js",
        "system/controllers/grab.js",
        "system/controllers/teleport.js",
        "system/controllers/toggleAdvancedMovementForHandControllers.js",
        "system/dialTone.js",
        "system/firstPersonHMD.js",
        "system/tablet-ui/tabletUI.js"
    ]);
} else {
    print('defaultScripts.js - isAndroid check IT IS ANDROID');
    pushAll(DEFAULT_SCRIPTS, [
        "system/progress.js",
        "system/away.js",
        "system/users.js",
        "system/mute.js",
        "system/goto.js",
        "system/hmd.js",
        "system/marketplaces/marketplace.js",
        //"system/edit.js",
        //"system/mod.js",
        //"system/selectAudioDevice.js",
        //"system/notifications.js",
        "system/controllers/controllerDisplayManager.js",
        "system/controllers/handControllerGrabAndroid.js",
        "system/controllers/handControllerPointerAndroid.js",
        "system/controllers/squeezeHands.js",
        "system/controllers/grab.js",
        "system/controllers/teleport.js",
        "system/controllers/toggleAdvancedMovementForHandControllers.js",
        "system/dialTone.js",
        "system/firstPersonHMD.js",
        "system/snapshot.js",
        "system/help.js",
        "system/bubble.js"
    ]);
}


// add a menu item for debugging
var MENU_CATEGORY = "Developer";
var MENU_ITEM = "Debug defaultScripts.js";

var SETTINGS_KEY = '_debugDefaultScriptsIsChecked';
var previousSetting = Settings.getValue(SETTINGS_KEY);

if (previousSetting === '' || previousSetting === false || previousSetting === 'false') {
    previousSetting = false;
}

if (previousSetting === true || previousSetting === 'true') {
    previousSetting = true;
}

if (Menu.menuExists(MENU_CATEGORY) && !Menu.menuItemExists(MENU_CATEGORY, MENU_ITEM)) {
    Menu.addMenuItem({
        menuName: MENU_CATEGORY,
        menuItemName: MENU_ITEM,
        isCheckable: true,
        isChecked: previousSetting,
        grouping: "Advanced"
    });
}

function runDefaultsTogether() {
    for (var j in DEFAULT_SCRIPTS) {
        Script.include(DEFAULT_SCRIPTS[j]);
    }
}

function runDefaultsSeparately() {
    for (var i in DEFAULT_SCRIPTS) {
        Script.load(DEFAULT_SCRIPTS[i]);
    }
}

// start all scripts
if (Menu.isOptionChecked(MENU_ITEM)) {
    // we're debugging individual default scripts
    // so we load each into its own ScriptEngine instance
    runDefaultsSeparately();
} else {
    // include all default scripts into this ScriptEngine
    runDefaultsTogether();
}

function menuItemEvent(menuItem) {


    if (menuItem === MENU_ITEM) {
        var isChecked = Menu.isOptionChecked(MENU_ITEM);
        if (isChecked === true) {
            Settings.setValue(SETTINGS_KEY, true);
        } else if (isChecked === false) {
            Settings.setValue(SETTINGS_KEY, false);
        }
        Menu.triggerOption("Reload All Scripts");
    }
}

function removeMenuItem() {
    if (!Menu.isOptionChecked(MENU_ITEM)) {
        Menu.removeMenuItem(MENU_CATEGORY, MENU_ITEM);
    }
}

Script.scriptEnding.connect(function() {
    removeMenuItem();
});

Menu.menuItemEvent.connect(menuItemEvent);
