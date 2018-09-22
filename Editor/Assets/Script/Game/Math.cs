using UnityEngine;
using System.Collections;

public static class GameMath
{
    public static int mod(int n, int x)
    {
        return ((n % x) + x) % x;
    }
}

public struct VecInt2
{
    public VecInt2(int x, int y)
    {
        this.x = x; this.y = y;
    }
    public int x;
    public int y;
}

public struct VecInt3
{
    public VecInt3(int x, int y, int z)
    {
        this.x = x; this.y = y; this.z = z;
    }
    public VecInt3(Vector3 pos)
    {
        this.x = (int)pos.x; this.y = (int)pos.y; this.z = (int)pos.z;
    }

    public static implicit operator Vector3(VecInt3 v)
    {
        return new Vector3(v.x, v.y, v.z);
    }
    public static implicit operator VecInt3(Vector3 v)
    {
        return new VecInt3((int)v.x, (int)v.y, (int)v.z);
    }

    public int x;
    public int y;
    public int z;
}

public struct RectInt
{
    public RectInt(int x, int y, int w, int h)
    {
        this.x = x; this.y = y; this.w = w; this.h = h;
    }
    public int x, y;
    public int w, h;
}

public class FakeRandom
{
    public static int Range(int minValue, int maxValue)
    {
        return Random.Range(minValue, maxValue);
    }

    public static float Range(float minValue, float maxValue)
    {
        return Random.Range(minValue, maxValue);
    }

    public static float Range01()
    {
        return Range(0.0f, 1.0f);
    }
}