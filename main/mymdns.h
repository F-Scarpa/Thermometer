#pragma once
#include "mdns.h"
#include "mymdns.h"

void start_mdns_service()
{
  mdns_init();
  mdns_hostname_set("thermo");    //create our name for server, set the host  type hostname.local
  mdns_instance_name_set("LEARN esp32 thing");
}

static const char *BASE_PATH = "/store";  
void mount_fs()     //setup for fat memory
{
   esp_vfs_fat_mount_config_t esp_vfs_fat_mount_config = {
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    esp_vfs_fat_spiflash_mount_ro(BASE_PATH, "storage", &esp_vfs_fat_mount_config);   //base_path is /store
                                                                                      //2. partition name in 
                                                                                      //partition table
}