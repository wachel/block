using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Block
{
    public struct RayCastRestult
    {
        public bool bHit;
        public float hitLength;
        public int hitFaceIndex;
    };

    public enum RayCastBlockType:byte
    {
        Nothing = 0,
        Selectable = 1<<0,
        Collider = 1<<1,
        Opacity = 1<<2,
        Walkable = 1<<3,
        All = 0xff,
    }

    public class RayCastManager
    {
        private int sizeX;
        private int sizeY;
        private int sizeZ;

        private int baseX;
        private int baseY;
        private int baseZ;
        RayCastBlockType[, ,] blocks;
        byte[,] airBlocksInSun;//能照到阳光的最下面的空气块
        byte[, ,] light;
        public RayCastManager() { }
        ~RayCastManager() { }
        public void create(int sizeX,int sizeY,int sizeZ)
        {
            this.sizeX = sizeX;
            this.sizeY = sizeY;
            this.sizeZ = sizeZ;
            blocks = new RayCastBlockType[sizeX, sizeY, sizeZ];
            airBlocksInSun = new byte[sizeX, sizeZ];
            light = new byte[sizeX ,sizeY, sizeZ];
        }

        public RayCastRestult rayCast(Vector3 startPos, Vector3 dir, float length, RayCastBlockType mask)
        {
            RayCastRestult outResult;
            //求局部坐标，左下角为(0,0,0),单位长度为chunkSize
            Vector3 localStartPos = startPos - new Vector3(baseX, baseY, baseZ);
            RayCastResult rlt = RayTrace.rayCast(localStartPos, dir, this, length, mask);//进行粗糙范围射线追踪
            outResult.bHit = rlt.bHit;
            if (rlt.hitLength > length) {
                outResult.bHit = false;
            }
            outResult.hitFaceIndex = (int)rlt.faceIndex;
            outResult.hitLength = rlt.hitLength;
            return outResult;
        }

        public RayCastRestult[] batchRayCast(Vector3[] startPos, Vector3[] dir, float[] lenght, RayCastBlockType mask)
        {
            int num = startPos.Length;
            RayCastRestult[] rlts = new RayCastRestult[num];
            for (int i = 0; i < num; i++) {
                rlts[i] = rayCast(startPos[i], dir[i], lenght[i], mask);
            }
            return rlts;
        }

        public void setBlock(int x, int y, int z, RayCastBlockType block)
        {
            if (y >= 0 && y < sizeY) {
                int lx = ((x % sizeX) + sizeX) % sizeX;
                int ly = y;
                int lz = ((z % sizeZ) + sizeZ) % sizeZ;
                blocks[lx, ly, lz] = block;
            }
        }

        public void updateInSun(int gx, int gz)
        {
            int lx = ((gx % sizeX) + sizeX) % sizeX;
            int lz = ((gz % sizeZ) + sizeZ) % sizeZ;
            for (int ly = sizeY - 1; ly >= 0; ly--) {
                if ((blocks[lx,ly,lz] & RayCastBlockType.Opacity) != 0) {
                    airBlocksInSun[lx, lz] = (byte)(ly + 1);
                    break;
                }
            }
        }
        public void updateAllLight()
        {
            updateLight(new VecInt3(0, 0, 0), new VecInt3(sizeX, sizeY, sizeZ));
        }
        public void updateLight(VecInt3 start,VecInt3 size)
        {
            List<VecInt3> extendSurface = new List<VecInt3>();
            for (int i = start.x; i < start.x + size.x; i++) {
                for (int j = start.y; j < start.y + size.y; j++) {
                    for (int k = start.z; k < start.z + size.z; k++) {
                        if (airBlocksInSun[i, k] <= j) {
                            light[i, j, k] = Const.MaxLightIteration;
                            bool x0InSun = airBlocksInSun[i == 0 ? sizeX - 1 : i - 1, k] <= j;
                            bool x1InSun = airBlocksInSun[i == sizeX - 1 ? 0 : i + 1, k] <= j;
                            bool z0InSun = airBlocksInSun[i, k == 0 ? sizeZ - 1 : k - 1] <= j;
                            bool z1InSun = airBlocksInSun[i, k == sizeZ - 1 ? 0 : k + 1] <= j;
                            if ((!x0InSun) || (!x1InSun) || (!z0InSun) || (!z1InSun)) {
                                extendSurface.Add(new VecInt3(i, j, k));
                            }
                        }
                    }
                }
            }

            byte currentLight = Const.MaxLightIteration - 1;
            while (currentLight > 0 && extendSurface.Count > 0) {
                //Debug.Log("light = " + currentLight + ", count = " + extendSurface.Count);
                List<VecInt3> newExtendSurface = new List<VecInt3>();
                for (int s = 0; s < extendSurface.Count; s++) {
                    VecInt3 v = extendSurface[s];
                    for (int f = 0; f < 6; f++) {
                        VecInt3 offset = Const.AdjacencyOffsetV[f];
                        int ax = v.x + offset.x;
                        int ay = v.y + offset.y;
                        int az = v.z + offset.z;
                        ax = ((ax % sizeX) + sizeX) % sizeX;
                        az = ((az % sizeZ) + sizeZ) % sizeZ;
                        if (ay >= 0 && ay < sizeY) {
                            if (light[ax,ay,az] < currentLight && ((blocks[ax, ay, az] & RayCastBlockType.Opacity) == 0)) {
                                newExtendSurface.Add(new VecInt3(ax, ay, az));
                                light[ax, ay, az] = (byte)currentLight;
                            }
                        }
                    }
                }
                extendSurface = newExtendSurface;
                currentLight -= 1;
            }
        }

        public void clearAll()
        {
            for (int i = 0; i < sizeX; i++) {
                for (int k = 0; k < sizeZ; k++) {
                    for (int j = 0; j < sizeY; j++) {
                        blocks[i, j, k] = RayCastBlockType.Nothing;
                    }
                    airBlocksInSun[i, k] = 0;
                }
            }
        }

        public bool testBlock(int x, int y, int z, RayCastBlockType mask)
        {
            if (y >= 0 && y < sizeY) {
                int lx = ((x % sizeX) + sizeX) % sizeX;
                int ly = y;
                int lz = ((z % sizeZ) + sizeZ) % sizeZ;
                return (((byte)blocks[lx, ly, lz]) & (byte)mask) != 0;
            }
            return false;
        }

        public bool isWalkable(int x, int y, int z,int height)
        {
            if (y >= 0 && y < sizeY) {
                int lx = ((x % sizeX) + sizeX) % sizeX;
                int ly = y;
                int lz = ((z % sizeZ) + sizeZ) % sizeZ;
                for (int i = 0; i < height; i++) {
                    bool bIsAir = (blocks[lx, ly + i, lz] & RayCastBlockType.Collider) == 0;
                    if (!bIsAir) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        }

        public bool isInSun(int x, int y,int z)
        {
            int lx = ((x % sizeX) + sizeX) % sizeX;
            int lz = ((z % sizeZ) + sizeZ) % sizeZ;
            return airBlocksInSun[lx, lz] <= y;
        }

        public byte getLight(int x,int y,int z)
        {
            if (y >= 0 && y < sizeY) {
                int lx = ((x % sizeX) + sizeX) % sizeX;
                int lz = ((z % sizeZ) + sizeZ) % sizeZ;
                return light[lx, y, lz];
            }
            return 0;
        }

        public int getSizeX() { return sizeX; }
        public int getSizeY() { return sizeY; }
        public int getSizeZ() { return sizeZ; }
        public Vector3 getSize() {
            return new Vector3(sizeX, sizeY, sizeZ);
        }
    };
}
