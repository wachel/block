using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;


public enum GameItemType
{
    None,

    Apple01,
    Apple02,
    Axe,
    Bat,
    Beer01,
    Beer02,
    Beer03,
    Cleaver,
    CrowBar,
    HealthPack,
}

public struct HouseItem
{
    public HouseItem(VecInt3 pos,GameItemType item){
        this.pos = pos;
        this.item = item;
    }
    public VecInt3 pos;
    public GameItemType item;
}

public class HouseItemGenerator 
{
    private static Dictionary<FloorType, List<GameItemType>> itemsConfig = new Dictionary<FloorType, List<GameItemType>>();
    private static Dictionary<GameItemType, string> itemsPrefab = new Dictionary<GameItemType,string>();
    public static void Init()
    {
        List<GameItemType> livingRoom = new List<GameItemType>();
        livingRoom.Add(GameItemType.Axe);
        livingRoom.Add(GameItemType.Bat);
        livingRoom.Add(GameItemType.CrowBar);
        livingRoom.Add(GameItemType.Beer01);
        livingRoom.Add(GameItemType.Beer02);
        livingRoom.Add(GameItemType.Beer03);

        List<GameItemType> kitchen = new List<GameItemType>();
        kitchen.Add(GameItemType.Apple01);
        kitchen.Add(GameItemType.Apple02);
        kitchen.Add(GameItemType.Beer01);
        kitchen.Add(GameItemType.Beer02);
        kitchen.Add(GameItemType.Beer03);
        kitchen.Add(GameItemType.Cleaver);

        itemsConfig[FloorType.LivingRoom] = livingRoom;
        itemsConfig[FloorType.Kitchen] = kitchen;
        itemsConfig[FloorType.Bathroom] = livingRoom;
        itemsConfig[FloorType.BedRoom0] = livingRoom;
        itemsConfig[FloorType.Bathroom] = livingRoom;

        itemsPrefab[GameItemType.Apple01]       = "Prefeb/Items/Items_Apple_01";
        itemsPrefab[GameItemType.Apple02]       = "Prefeb/Items/Items_Apple_02";
        itemsPrefab[GameItemType.Axe]           = "Prefeb/Items/Items_Axe_01";
        itemsPrefab[GameItemType.Bat]           = "Prefeb/Items/Items_Bat_01";
        itemsPrefab[GameItemType.Beer01]        = "Prefeb/Items/Items_Beer_01";
        itemsPrefab[GameItemType.Beer02]        = "Prefeb/Items/Items_Beer_02";
        itemsPrefab[GameItemType.Beer03]        = "Prefeb/Items/Items_Beer_03";
        itemsPrefab[GameItemType.Cleaver]       = "Prefeb/Items/Items_Cleaver_01";
        itemsPrefab[GameItemType.CrowBar]       = "Prefeb/Items/Items_CrowBar_01";
        itemsPrefab[GameItemType.HealthPack]    = "Prefeb/Items/Items_HealthPack_01";
    }

    public static string GetItemPrefabPath(GameItemType item)
    {
        return itemsPrefab.ContainsKey(item) ? itemsPrefab[item] : "";
    }
    public static List<HouseItem> GenerateItems(FloorType[,] layout, int[,] distanceToWall)
    {
        int w = layout.GetLength(0);
        int h = layout.GetLength(1);
        List<HouseItem> rlt = new List<HouseItem>();
        
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (itemsConfig.ContainsKey(layout[i,j]) && distanceToWall[i, j] == 1) {
                    if (FakeRandom.Range01() < 0.3f) {
                        List<GameItemType>items = itemsConfig[layout[i,j]];
                        GameItemType randomItem = items[FakeRandom.Range(0,items.Count)];
                        rlt.Add(new HouseItem(new VecInt3(i,1,j),randomItem));
                    }
                }
            }
        }
        return rlt;
    }

}
