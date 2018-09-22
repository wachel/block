using UnityEngine;
using System.Collections;
using System.Collections.Generic;



public class LayoutToBuilding
{
    public static FloorType[,] LayoutCellToLine(FloorType[,] layout)
    {
        int w = layout.GetLength(0);
        int h = layout.GetLength(1);
        int minX = w;
        int minY = h;
        int maxX = 0;
        int maxY = 0;
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (layout[i, j] != FloorType.Out) {
                    minX = i < minX ? i : minX;
                    minY = j < minY ? j : minY;
                    maxX = i > maxX ? i : maxX;
                    maxY = j > maxY ? j : maxY;
                }
            }
        }
        minX -= 2;
        minY -= 2;
        maxX += 3;
        maxY += 3;
        RectInt range = new RectInt(minX, minY, maxX - minX + 1, maxY - minY + 1);
        FloorType[,] rlt = new FloorType[range.w, range.h];
        for (int i = 0; i < range.w; i++) {
            for (int j = 0; j < range.h; j++) {
                int sx = minX + i;
                int sy = minY + j;
                if (sx - 1 < 0 || sx - 1 >= layout.GetLength(0) || sy - 1 < 0 || sy - 1 >= layout.GetLength(1)) {
                    //int a = 0;
                }
                FloorType leftDown = layout[sx - 1, sy - 1];
                FloorType rightDown = layout[sx, sy - 1];
                FloorType leftUp = layout[sx - 1, sy];
                FloorType rightUp = layout[sx, sy];

                FloorType newBlock = FloorType.Wall;

                if (leftDown == rightDown && leftDown == leftUp && leftDown == rightUp) {//内部区域
                    newBlock = leftDown;
                }
                else if (leftUp == FloorType.DoorUp || rightUp == FloorType.DoorUp) {//朝上的门
                    newBlock = FloorType.DoorUp;
                }
                else if (leftDown == FloorType.DoorUp) {
                    newBlock = leftUp;
                }
                else if (rightDown == FloorType.DoorUp) {
                    newBlock = rightUp;
                }
                else if (leftDown == FloorType.DoorDown || rightDown == FloorType.DoorDown) {//朝下的门
                    newBlock = FloorType.DoorDown;
                }
                else if (leftUp == FloorType.DoorDown) {
                    newBlock = leftDown;
                }
                else if (rightUp == FloorType.DoorDown) {
                    newBlock = rightDown;
                }
                else if (leftDown == FloorType.DoorLeft || leftUp == FloorType.DoorLeft) {//朝左的门
                    newBlock = FloorType.DoorLeft;
                }
                else if (rightDown == FloorType.DoorLeft) {
                    newBlock = leftDown;
                }
                else if (rightUp == FloorType.DoorLeft) {
                    newBlock = leftUp;
                }
                else if (rightDown == FloorType.DoorRight || rightUp == FloorType.DoorRight) {//朝右的门
                    newBlock = FloorType.DoorRight;
                }
                else if (leftDown == FloorType.DoorRight) {
                    newBlock = rightDown;
                }
                else if (leftUp == FloorType.DoorRight) {
                    newBlock = rightUp;
                }
                else {
                    newBlock = FloorType.Wall;
                }
                rlt[i, j] = newBlock;
            }
        }
        return rlt;
    }

    public static void AddWindow(FloorType[,] layout)
    {
        int w = layout.GetLength(0);
        int h = layout.GetLength(1);
        for (int j = 1; j < h - 1; j++) {
            bool bWaitWallStart = true;
            bool bWaitWallEnd = false;
            VecInt2 wallStart = new VecInt2();
            VecInt2 wallEnd = new VecInt2();
            for (int i = 1; i < w - 1; i++) {
                if (bWaitWallStart) {
                    if (layout[i, j] == FloorType.Wall) {
                        bWaitWallStart = false;
                        bWaitWallEnd = true;
                        wallStart = new VecInt2(i, j);
                        continue;
                    }
                }
                if (bWaitWallEnd) {
                    if (layout[i, j] != FloorType.Wall || layout[i, j + 1] == FloorType.Wall || layout[i, j - 1] == FloorType.Wall) {
                        bWaitWallEnd = false;
                        bWaitWallStart = true;
                        wallEnd = new VecInt2(i, j);
                        AddWindowOnHorizonWall(layout, wallStart, wallEnd);
                        continue;
                    }
                }
            }
        }

        for (int i = 1; i < w - 1; i++) {
            bool bWaitWallStart = true;
            bool bWaitWallEnd = false;
            VecInt2 wallStart = new VecInt2();
            VecInt2 wallEnd = new VecInt2();
            for (int j = 1; j < h - 1; j++) {
                if (bWaitWallStart) {
                    if (layout[i, j] == FloorType.Wall) {
                        bWaitWallStart = false;
                        bWaitWallEnd = true;
                        wallStart = new VecInt2(i, j);
                        continue;
                    }
                }
                if (bWaitWallEnd) {
                    if (layout[i, j] != FloorType.Wall || layout[i + 1, j] == FloorType.Wall || layout[i - 1, j] == FloorType.Wall) {
                        bWaitWallEnd = false;
                        bWaitWallStart = true;
                        wallEnd = new VecInt2(i, j);
                        AddWindowOnVerticalWall(layout, wallStart, wallEnd);
                        continue;
                    }
                }
            }
        }

    }

    private static void AddWindowOnHorizonWall(FloorType[,] layout, VecInt2 wallStart, VecInt2 wallEnd)
    {
        int length = wallEnd.x - wallStart.x;
        if (length < 6) {

        }
        else if (length < 12) {
            AddOneWindow(layout, wallStart.x + length / 2 - 1, wallStart.y);
            AddOneWindow(layout, wallStart.x + length / 2, wallStart.y);
            AddOneWindow(layout, wallStart.x + length / 2 + 1, wallStart.y);
        }
        else if (length < 18) {
            AddOneWindow(layout, wallStart.x + length / 3, wallStart.y);
            AddOneWindow(layout, wallStart.x + length / 3 + 1, wallStart.y);
            AddOneWindow(layout, wallStart.x + length * 2 / 3, wallStart.y);
            AddOneWindow(layout, wallStart.x + length * 2 / 3 + 1, wallStart.y);
        }
        else {
            AddOneWindow(layout, wallStart.x + length / 3 - 1, wallStart.y);
            AddOneWindow(layout, wallStart.x + length / 3, wallStart.y);
            AddOneWindow(layout, wallStart.x + length / 3 + 1, wallStart.y);
            AddOneWindow(layout, wallStart.x + length * 2 / 3 - 1, wallStart.y);
            AddOneWindow(layout, wallStart.x + length * 2 / 3, wallStart.y);
            AddOneWindow(layout, wallStart.x + length * 2 / 3 + 1, wallStart.y);
        }
    }

    private static void AddWindowOnVerticalWall(FloorType[,] layout, VecInt2 wallStart, VecInt2 wallEnd)
    {
        int length = wallEnd.y - wallStart.y;
        if (length < 5) {

        }
        else if (length < 10) {
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 2 - 1);
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 2);
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 2 + 1);
        }
        else if (length < 16) {
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 3);
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 3 + 1);
            AddOneWindow(layout, wallStart.x, wallStart.y + length * 2 / 3);
            AddOneWindow(layout, wallStart.x, wallStart.y + length * 2 / 3 + 1);
        }
        else {
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 3 - 1);
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 3);
            AddOneWindow(layout, wallStart.x, wallStart.y + length / 3 + 1);
            AddOneWindow(layout, wallStart.x, wallStart.y + length * 2 / 3 - 1);
            AddOneWindow(layout, wallStart.x, wallStart.y + length * 2 / 3);
            AddOneWindow(layout, wallStart.x, wallStart.y + length * 2 / 3 + 1);
        }
    }

    private static void AddOneWindow(FloorType[,] layout, int x, int y)
    {
        if (layout[x - 1, y] == FloorType.Out || layout[x + 1, y] == FloorType.Out || layout[x, y - 1] == FloorType.Out || layout[x, y + 1] == FloorType.Out) {
            layout[x, y] = FloorType.Window;
        }
    }

    public static int[,] GetDistanceToWall(FloorType[,] layout)
    {
        int w = layout.GetLength(0);
        int l = layout.GetLength(1);

        int[,] rlt = new int[w, l];
        const int Max = 99;
        int curValue = Max;

        List<VecInt2> maxCells = new List<VecInt2>();
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < l; j++) {
                //rlt[i, j] = layout[i, j] == FloorType.Wall ? Max : 0;
                if (layout[i, j] == FloorType.Wall) {
                    rlt[i, j] = curValue;
                    maxCells.Add(new VecInt2(i, j));
                }
            }
        }

        while (maxCells.Count > 0) {
            List<VecInt2> newCells = new List<VecInt2>();
            for (int c = 0; c < maxCells.Count; c++) {
                VecInt2 cell = maxCells[c];
                int value = rlt[cell.x, cell.y];
                if (cell.x > 0 && rlt[cell.x - 1, cell.y] < value - 1) {
                    rlt[cell.x - 1, cell.y] = value - 1;
                    newCells.Add(new VecInt2(cell.x - 1, cell.y));
                }

                if (cell.x < w - 1 && rlt[cell.x + 1, cell.y] < value - 1) {
                    rlt[cell.x + 1, cell.y] = value - 1;
                    newCells.Add(new VecInt2(cell.x + 1, cell.y));
                }

                if (cell.y > 0 && rlt[cell.x, cell.y - 1] < value - 1) {
                    rlt[cell.x, cell.y - 1] = value - 1;
                    newCells.Add(new VecInt2(cell.x, cell.y - 1));
                }

                if (cell.y < l - 1 && rlt[cell.x, cell.y + 1] < value - 1) {
                    rlt[cell.x, cell.y + 1] = value - 1;
                    newCells.Add(new VecInt2(cell.x, cell.y + 1));
                }
            }
            maxCells = newCells;
        }


        for (int i = 0; i < w; i++) {
            for (int j = 0; j < l; j++) {
                rlt[i, j] = Max - rlt[i, j];
            }
        }

        return rlt;
    }

    public static Game.BlockType[, ,] CreateBuilding(FloorType[,] layout, bool addRoof)
    {
        int w = layout.GetLength(0);
        int h = 7;
        int roofHeight = 8;
        int l = layout.GetLength(1);

        Game.BlockType[, ,] rlt = new Game.BlockType[w, h + roofHeight, l];

        for (int i = 0; i < w; i++) {
            for (int j = 0; j < l; j++) {
                FloorType floor = layout[i, j];

                FloorType left = i == 0 ? FloorType.Out : layout[i - 1, j];
                FloorType right = i == w - 1 ? FloorType.Out : layout[i + 1, j];
                FloorType back = j == 0 ? FloorType.Out : layout[i, j - 1];
                FloorType front = j == l - 1 ? FloorType.Out : layout[i, j + 1];

                if (floor == FloorType.Out) {
                    if (left == FloorType.DoorLeft) {
                        rlt[i, 0, j] = Game.BlockType.StairRight;
                    }
                    else if (right == FloorType.DoorRight) {
                        rlt[i, 0, j] = Game.BlockType.StairLeft;
                    }
                    else if (back == FloorType.DoorDown) {
                        rlt[i, 0, j] = Game.BlockType.StairFront;
                    }
                    else if(front == FloorType.DoorUp) {
                        rlt[i, 0, j] = Game.BlockType.StairBack;
                    }
                    //|| right == FloorType.DoorRight || up == FloorType.DoorUp || down == FloorType.DoorDown) {
                    //    rlt[i, 0, j] = Game.BlockType.WoolColoredBlack;
                    //}
                }
                else if (floor == FloorType.Wall || floor == FloorType.Window || HouseLayout.IsDoor(floor)) {
                    for (int k = 0; k < h; k++) {
                        if (left == FloorType.Out) {
                            rlt[i, k, j] = Game.BlockType.StoneBrickWhiteWall1;
                        }
                        else if (right == FloorType.Out) {
                            rlt[i, k, j] = Game.BlockType.StoneBrickWhiteWall0;
                        }
                        else if (front == FloorType.Out) {
                            rlt[i, k, j] = Game.BlockType.StoneBrickWhiteWall4;
                        }
                        else if (back == FloorType.Out) {
                            rlt[i, k, j] = Game.BlockType.StoneBrickWhiteWall5;
                        }
                        else {
                            rlt[i, k, j] = Game.BlockType.WhiteStone;
                        }
                    }
                    if (floor == FloorType.Window) {
                        for (int k = 3; k < 5; k++) {
                            rlt[i, k, j] = Game.BlockType.Air;
                        }
                    }
                    else if (HouseLayout.IsDoor(floor)) {
                        rlt[i, 0, j] = Game.BlockType.WoolColoredBlack;
                        for (int k = 1; k < 5; k++) {
                            rlt[i, k, j] = Game.BlockType.Air;
                        }
                    }
                }
                else {
                    if (floor == FloorType.Kitchen || floor == FloorType.Bathroom) {
                        rlt[i, 0, j] = Game.BlockType.StoneSlab;
                    }
                    else {
                        rlt[i, 0, j] = Game.BlockType.WoodFloor;
                    }
                }
            }
        }
        //生成屋顶
        if (addRoof) {
            bool[,] blocks = new bool[w, l];
            bool[,] blocksOld = new bool[w, l];
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < l; j++) {
                    blocks[i, j] = layout[i, j] != FloorType.Out;
                    blocksOld[i, j] = blocks[i, j];
                }
            }
            for (int i = 1; i < w - 1; i++) {
                for (int j = 1; j < l - 1; j++) {
                    blocks[i, j] = blocksOld[i, j] || blocksOld[i - 1, j] || blocksOld[i, j - 1] || blocksOld[i + 1, j] || blocksOld[i, j + 1]
                                                    || blocksOld[i - 1, j - 1] || blocksOld[i - 1, j + 1] || blocksOld[i + 1, j - 1] || blocksOld[i + 1, j + 1];
                }
            }

            for (int i = 0; i < w; i++) {
                for (int j = 0; j < l; j++) {
                    blocksOld[i, j] = blocks[i, j];
                }
            }

            Game.BlockType[] roofTypes = { Game.BlockType.HardenedClayStainedBlue, Game.BlockType.RedSand };
            Game.BlockType roofType = roofTypes[FakeRandom.Range(0, roofTypes.Length)];

            for (int k = 0; k < roofHeight; k++) {
                for (int i = 0; i < w; i++) {
                    for (int j = 0; j < l; j++) {
                        if (blocks[i, j]) {
                            if (k == 0) {
                                rlt[i, k + h, j] = layout[i, j] == FloorType.Out ? roofType : Game.BlockType.WhiteStone;
                            }
                            else {
                                rlt[i, k + h, j] = roofType;
                            }
                        }
                    }
                }
                bool[,] temp = blocksOld;
                blocksOld = blocks;
                blocks = temp;
                for (int i = 1; i < w - 1; i++) {
                    for (int j = 1; j < l - 1; j++) {
                        blocks[i, j] = blocksOld[i, j] && blocksOld[i - 1, j] && blocksOld[i, j - 1] && blocksOld[i + 1, j] && blocksOld[i, j + 1];
                    }
                }
            }
        }
        return rlt;
    }
}
