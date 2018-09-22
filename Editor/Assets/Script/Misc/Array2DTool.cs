using UnityEngine;
using System.Collections;


public class Array2DTool
{
    public static void SetOne<T>(T[,] blocks, int x, int y, T block)
    {
        blocks[x, y] = block;
    }
    public static void SetRange<T>(T[,] blocks, int x, int y, int w, int h, T block)
    {
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                blocks[x + i, y + j] = block;
            }
        }
    }
    public static void SetRange<T>(T[,] blocks, RectInt range, T block)
    {
        SetRange(blocks, range.x, range.y, range.w, range.h, block);
    }
    public static void SetAll<T>(T[,] blocks, T block)
    {
        SetRange(blocks, 0, 0, blocks.GetLength(0), blocks.GetLength(1), block);
    }
    public static bool RangeIsAll<T>(T[,] blocks, RectInt range, T block)
    {
        for (int i = 0; i < range.w; i++) {
            for (int j = 0; j < range.h; j++) {
                if (range.x + i < 0 || range.x + i >= blocks.GetLength(0) || range.y + j < 0 || range.y + j >= blocks.GetLength(1)) {
                    return false;
                }
                if (!blocks[range.x + i, range.y + j].Equals(block)) {
                    return false;
                }
            }
        }
        return true;
    }
    public static string GetDebugString<T>(T[,] blocks,string split = "")
    {
        string rlt = "";
        int w = blocks.GetLength(0);
        int h = blocks.GetLength(1);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                rlt += (blocks[i, h - j - 1]).ToString() + (i == w - 1?"":split);
            }
            rlt += "\n";
        }
        return rlt;
    }
    public static string GetDebugStringFormat<T>(T[,] blocks, string format,string split)
    {
        string rlt = "";
        int w = blocks.GetLength(0);
        int h = blocks.GetLength(1);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                rlt += string.Format(format,blocks[i, h - j - 1]) + (i == w - 1 ? "" : split);
            }
            rlt += "\n";
        }
        return rlt;
    }
}