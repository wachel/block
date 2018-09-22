using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public enum FurnitureType
{
    Door,
    Window,
    Bed,
    Table,
    Chair,
    Closet,     //壁橱
    Sofa,
    Desk,
    Cupboard,   //碗柜
    Chest,      //衣柜
}

public class Furniture
{
    public Furniture(){

    }
    public int dx,dy,dz;
    Game.BlockType[,,]blocks;
    List<GameItemType> items;
}

public class LayoutFurniture {
    //public static VecInt
}
