using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public enum FloorType
{
    Out,
    Wall,//墙
    Window,//窗.墙和窗不在layout中使用，只用来生成建筑
    DoorLeft,
    DoorRight,
    DoorUp,
    DoorDown,
    LivingRoom,
    BedRoom0,
    BedRoom1,
    BedRoom2,
    Kitchen,
    DiningRoom,
    Bathroom,
}


public enum Direction
{
    Left,
    Right,
    Up,
    Down,
}


public class Room
{
    public Room(VecInt2 doorPos, FloorType type,int targetSize)
    {
        this.doorPos = doorPos; this.type = type; this.targetSize = targetSize;
    }
    public List<Room> children = new List<Room>();
    public VecInt2 doorPos;
    public Direction dir;
    public RectInt range;
    public FloorType type;
    public int targetSize;
    public BlueprintNode blueprint;
    public int GetCurrentSize()
    {
        return range.w * range.h;
    }
}

public enum HouseType{
    Big,
    Middle,
    Small,
}

public class BlueprintNode
{
    public BlueprintNode(FloorType type, int sizeMin, int sizeMax, int edgeLenghtMin)
    {
        this.type = type;
        this.sizeMin = sizeMin; this.sizeMax = sizeMax; this.edgeLenghtMin = edgeLenghtMin;
    }
    public FloorType type;
    public int sizeMin;
    public int sizeMax;
    public int edgeLenghtMin;
    public List<BlueprintNode> children = new List<BlueprintNode>();
}

public class HouseLayout 
{
    public static readonly int Gap = 3;
    public static FloorType[] doorType = { FloorType.DoorLeft, FloorType.DoorRight, FloorType.DoorUp, FloorType.DoorDown };
    public static BlueprintNode GetRoomLink(HouseType type)
    {
        BlueprintNode livingRoom = new BlueprintNode(FloorType.LivingRoom,100,200,8);
        livingRoom.children.Add(new BlueprintNode(FloorType.BedRoom0,60,100,6));
        livingRoom.children.Add(new BlueprintNode(FloorType.BedRoom1,60,100,6));
        livingRoom.children.Add(new BlueprintNode(FloorType.Bathroom,40,60,4));
        livingRoom.children.Add(new BlueprintNode(FloorType.Kitchen,40,60,4));
        return livingRoom;
    }

    public static FloorType[,] CreateHouse(HouseType type,Direction dir)
    {
        int mapW = 50;
        int mapH = 50;
        BlueprintNode blueprint = GetRoomLink(type);
        FloorType[,] blocks = new FloorType[mapW,mapH];
        Room livingRoom = CreateMinRoom(ref blocks, blueprint, new VecInt2(mapW / 2, mapH / 2), dir);
        while (livingRoom.GetCurrentSize() < livingRoom.targetSize) {
            bool bExtendOk = Extend(ref blocks, livingRoom);
            if (!bExtendOk) {
                break;
            }
        }
        livingRoom.children = CreateChildren(ref blocks,livingRoom,blueprint);
        return blocks; 
    }

    public static Room CreateMinRoom(ref FloorType[,]blocks, BlueprintNode blueprint, VecInt2 doorPos, Direction doorDir)
    {
        Room room = new Room(doorPos, blueprint.type, FakeRandom.Range(blueprint.sizeMin, blueprint.sizeMax));
        room.range = GetRange(doorPos, blueprint.edgeLenghtMin, blueprint.edgeLenghtMin, doorDir);
        room.dir = doorDir;
        Array2DTool.SetRange(blocks, room.range, blueprint.type);
        Array2DTool.SetOne(blocks, room.doorPos.x, room.doorPos.y, doorType[(int)room.dir]);
        return room;
    }

    public static List<Room> CreateChildren(ref FloorType[,] blocks, Room room, BlueprintNode blueprint)
    {
        List<Room> children = new List<Room>();
        for (int i = 0; i < blueprint.children.Count; i++) {
            BlueprintNode childBlueprint = blueprint.children[i];
            VecInt2 childDoorPos = new VecInt2(0, 0);
            Direction childDoorDir = Direction.Left;
            bool bValid = false;
            Direction[] dirs = GetExtendDirections(room.dir);//得到可以产生门的方向
            dirs = RandomSort(dirs, FakeRandom.Range(0, 10000));//打乱顺序
            foreach (Direction d in dirs) {
                for (int t = 0; t < 5; t++) {
                    childDoorPos = GetRandomChildDoorPos(room.range, d);//在随机位置生生成一个门
                    childDoorDir = d;
                    RectInt childMinRange = GetRange(childDoorPos, childBlueprint.edgeLenghtMin, childBlueprint.edgeLenghtMin, d);
                    if (Array2DTool.RangeIsAll(blocks, childMinRange, FloorType.Out)) {//如果是空地
                        bValid = true;//则找到了一个有效位置
                        break;
                    }
                }
                if (bValid) {
                    break;
                }
            }
            if (bValid) {//如果找到了一个有效的位置
                Room childRoom = CreateMinRoom(ref blocks,childBlueprint, childDoorPos, childDoorDir);//在此位置生成一个最小房间
                if (childRoom != null) {
                    childRoom.blueprint = childBlueprint;
                    children.Add(childRoom);
                }
            }
        }

        while (true) {
            bool bExtendOne = false;
            for (int i = 0; i < children.Count; i++) {
                if (children[i].GetCurrentSize() < children[i].targetSize) {//如果房间大小还没到期望值
                    bool bExtendOk = Extend(ref blocks, children[i]);//则往随机方向扩大一格
                    bExtendOne |= bExtendOk;
                }
            }
            if (!bExtendOne) {//如果没有任何房间被扩大过，则退出循环
                break;
            }
        }

        //给扩展过的子房间产生下一级房间
        for (int i = 0; i < children.Count; i++) {
            children[i].children = CreateChildren(ref blocks, children[i], children[i].blueprint);
        }

        return children;
    }

    public static VecInt2 GetRandomChildDoorPos(RectInt roomRange,Direction childDoorDir)
    {
        switch (childDoorDir) {
            case Direction.Left: return new VecInt2(roomRange.x - 1, Random.Range(roomRange.y + 2, roomRange.y + roomRange.h - 2));
            case Direction.Right: return new VecInt2(roomRange.x + roomRange.w, Random.Range(roomRange.y + 2, roomRange.y + roomRange.h - 2));
            case Direction.Up: return new VecInt2(Random.Range(roomRange.x + 2, roomRange.x + roomRange.w - 2),roomRange.y + roomRange.h);
            default: return new VecInt2(Random.Range(roomRange.x + 2, roomRange.x + roomRange.w - 2),roomRange.y - 1);
        }
    }
    public static RectInt GetRange(VecInt2 doorPos, int w, int h, Direction dir)
    {
        RectInt range;
        if (dir == Direction.Up) {
            range = new RectInt(doorPos.x - w / 2, doorPos.y, w, h);
        }
        else if (dir == Direction.Down) {
            range = new RectInt(doorPos.x - w / 2, doorPos.y - h + 1, w, h);
        }
        else if (dir == Direction.Left) {
            range = new RectInt(doorPos.x - w + 1, doorPos.y - h / 2, w, h);
        }
        else {//Right
            range = new RectInt(doorPos.x, doorPos.y - h / 2, w, h);
        }
        return range;
    }

    public static bool Extend(ref FloorType[,] blocks, Room room)
    {
        Direction[] dirs = GetExtendDirections(room.dir);
        dirs = RandomSort(dirs, FakeRandom.Range(0, 6));
        for (int i = 0; i < 3; i++) {
            Direction dir = dirs[i];
            if (dir == Direction.Left) {
                RectInt range = new RectInt(room.range.x - 1, room.range.y, 1, room.range.h);
                if (range.x >= Gap && Array2DTool.RangeIsAll(blocks, range, FloorType.Out)) {//边上留空Gap空隙，为了计算方便
                    room.range.x -= 1;
                    room.range.w += 1;
                    Array2DTool.SetRange(blocks, range, room.type);
                    return true;
                }
            }
            else if (dir == Direction.Right) {
                RectInt range = new RectInt(room.range.x + room.range.w, room.range.y, 1, room.range.h);
                if (range.x < blocks.GetLength(0) - Gap && Array2DTool.RangeIsAll(blocks, range, FloorType.Out)) {
                    room.range.w += 1;
                    Array2DTool.SetRange(blocks, range, room.type);
                    return true;
                }
            }
            else if (dir == Direction.Down) {
                RectInt range = new RectInt(room.range.x, room.range.y - 1, room.range.w, 1);
                if (range.y >= Gap && Array2DTool.RangeIsAll(blocks, range, FloorType.Out)) {
                    room.range.y -= 1;
                    room.range.h += 1;
                    Array2DTool.SetRange(blocks, range, room.type);
                    return true;
                }
            }
            else if (dir == Direction.Up) {
                RectInt range = new RectInt(room.range.x, room.range.y + room.range.h, room.range.w, 1);
                if (range.y < blocks.GetLength(1) - Gap && Array2DTool.RangeIsAll(blocks, range, FloorType.Out)) {
                    room.range.h += 1;
                    Array2DTool.SetRange(blocks, range, room.type);
                    return true;
                }
            }
        }
        return false;
    }
    public static Direction[] GetExtendDirections(Direction doorDirection)
    {
        switch (doorDirection) {
            case Direction.Left: return new Direction[] { Direction.Left, Direction.Up, Direction.Down };
            case Direction.Right: return new Direction[] { Direction.Right, Direction.Up, Direction.Down };
            case Direction.Up: return new Direction[] { Direction.Right, Direction.Left, Direction.Up };
            default: return new Direction[] { Direction.Right, Direction.Down, Direction.Left };
        }
    }
    public static Direction[] RandomSort(Direction[] dirs, int randKey)
    {
        if (dirs.Length == 3) {
            int r = randKey % 6;
            int[,] indices = { { 0, 1, 2 }, { 0, 2, 1 }, { 1, 0, 2 }, { 1, 2, 0 }, { 2, 0, 1 }, { 2, 1, 0 } };
            return new Direction[] { dirs[indices[r, 0]], dirs[indices[r, 1]], dirs[indices[r, 2]] };
        }
        return dirs;
    }

    public static bool IsDoor(FloorType type)
    {
        return type == FloorType.DoorLeft || type == FloorType.DoorRight || type == FloorType.DoorUp || type == FloorType.DoorDown; 
    }
}

public class BuildingDebugTools
{
    public static char GetBlockChar(FloorType type)
    {
        switch (type) {
            case FloorType.Out: return '－';
            case FloorType.Wall: return '墙';
            case FloorType.Window: return '窗';
            case FloorType.DoorLeft: return '门';
            case FloorType.DoorRight: return '门';
            case FloorType.DoorDown: return '门';
            case FloorType.DoorUp: return '门';
            case FloorType.LivingRoom: return '厅';
            case FloorType.BedRoom0: return '卧';
            case FloorType.BedRoom1: return '室';
            case FloorType.BedRoom2: return '内';
            case FloorType.Kitchen: return '厨';
            case FloorType.DiningRoom: return '餐';
            case FloorType.Bathroom: return '厕';
            default: return '？';
        }
    }
    public static string GetLayoutString(FloorType[,]blocks)
    {
        string rlt = "";
        int w = blocks.GetLength(0);
        int h = blocks.GetLength(1);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                rlt += GetBlockChar(blocks[i, h - j - 1]);
            }
            rlt += "\n";
        }
        return rlt;
    }
}