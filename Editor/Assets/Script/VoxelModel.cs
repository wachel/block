using UnityEngine;
using System.Collections;

[System.Serializable]
public class VoxelModel {
    [HideInInspector] 
    public short[] blocks;
    public int sizeX, sizeY, sizeZ;
    public int originX, originY, originZ;
}
