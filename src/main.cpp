 /**
  * Copyright (C) 2014, Jaguar Land Rover
  *
  * Author: Jonatan Palsson <jonatan.palsson@pelagicore.com>
  *
  * This file is part of the GENIVI Media Manager Proof-of-Concept
  * For further information, see http://genivi.org/
  *
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
  */

#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include <gio/gio.h>
#include <CommonAPI/CommonAPI.h>

#include "browserprovider.h"
#include "playerprovider.h"
#include "browserstub.h"
#include "playerstub.h"

int main (int argc, char *argv[]) {
    guint watcher_id;
    GMainLoop *loop;
    BrowserProvider browser;
    PlayerProvider player;

    static std::shared_ptr<BrowserStubImpl> browserService = std::make_shared<BrowserStubImpl>(&browser);
    static std::shared_ptr<PlayerStubImpl> playerService = std::make_shared<PlayerStubImpl>(&player);

    CommonAPI::Runtime::LoadState loadState;
    static auto runtime = CommonAPI::Runtime::load(loadState);
    if (loadState != CommonAPI::Runtime::LoadState::SUCCESS) {
        std::cerr << "Error: Unable to load runtime!\n";
        return -1;
    }

    browser.connect([&](MmError *e) {
      runtime->getServicePublisher()->registerService(browserService, 
        "local:org.genivi.mediamanager.Browser:org.genivi.mediamanager.Browser", runtime->createFactory());
    });

    player.connect([&](MmError *e) {
      runtime->getServicePublisher()->registerService(playerService, 
        "local:org.genivi.mediamanager.Player:org.genivi.mediamanager.Player", runtime->createFactory());
    });

    loop = g_main_loop_new (NULL, FALSE);
    g_main_loop_run (loop);
}
