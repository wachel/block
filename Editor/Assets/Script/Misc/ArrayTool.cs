using UnityEngine;
using System.Collections;

public class ArrayTool
{
    public static void SetAll<T>(T[] array, T value)
    {
        for (int i = 0; i < array.Length; i++) {
            array[i] = value;
        }
    }
}
