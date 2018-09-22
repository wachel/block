using UnityEngine;
using System.Collections;

namespace Block
{
    public enum Face:sbyte
    {
        FNI_Unknown = -1,
        FNI_x0,
        FNI_x1,
        FNI_y0,
        FNI_y1,
        FNI_z0,
        FNI_z1,
        FNI_OutOfLength,
    };

    struct RayTraceResult
    {
        //public RayTraceResult() { }
        public RayTraceResult(bool bHit, int x, int y, int z, Face faceIndex, float hitLength)
        {
            this.bHit = bHit;
            this.x = x;
            this.y = y;
            this.z = z;
            this.faceIndex = faceIndex;
            this.hitLength = hitLength;
        }
        public bool bHit;//True:碰到方块，False:离开
        public int x, y, z;//表示击中方块位置，无碰撞时此位置会在chunk外面
        public Face faceIndex;//碰撞法线朝向的面
        public float hitLength;//击中物体时碰撞线的长度，无碰撞时，会到达Chunk外面
    };

    class RayTrace
    {
        public static RayTraceResult rayTrace(Vector3 startPos, Vector3 dir, RayTraceChunkManager rtm, float length)
        {
            DirInfo dirInfo = new DirInfo(dir);
            Vector3 startPosInner = startPos;
            float hitTime = 0;
            float startOffset = 0;
            bool bHitBounds = true;//能击中总区域，或者在内部
            if (startPos.x >= 0 && startPos.x < rtm.getSizeX() && startPos.y >= 0 && startPos.y < rtm.getSizeY() && startPos.z >= 0 && startPos.z < rtm.getSizeZ()) {
                bHitBounds = true;
            }
            else {
                bHitBounds = Misc.rayHitAABB(startPos, dir, new Vector3(0, 0, 0), rtm.getSize(), ref hitTime);
                if (hitTime > 0) {
                    startOffset = hitTime;
                }
                startPosInner = startPos + dir * startOffset - dir * 0.01f;
            }
            if (bHitBounds) {
                RayTraceResult rlt = rayTraceSmall(startPosInner, dirInfo, rtm, Face.FNI_Unknown, length - startOffset);
                rlt.hitLength += startOffset;
                return rlt;
            }
            else {
                return new RayTraceResult(false, 0, 0, 0, Face.FNI_Unknown, 0);
            }
            
        }
        public struct DirInfo
        {
            public DirInfo(Vector3 dir)
            {
                float reciprocalX = dir.x == 0 ? 99999999.0f : 1 / dir.x;
                float reciprocalY = dir.y == 0 ? 99999999.0f : 1 / dir.y;
                float reciprocalZ = dir.z == 0 ? 99999999.0f : 1 / dir.z;

                stepX = dir.x >= 0 ? 1 : -1;
                stepY = dir.y >= 0 ? 1 : -1;
                stepZ = dir.z >= 0 ? 1 : -1;
                tDeltaX = Mathf.Abs(reciprocalX);
                tDeltaY = Mathf.Abs(reciprocalY);
                tDeltaZ = Mathf.Abs(reciprocalZ);

                this.dir = dir;
            }
            public int stepX;	//1或-1，表示射线的方向
            public int stepY;
            public int stepZ;
            public float tDeltaX;//每跨过1个X需要直线前进多少距离
            public float tDeltaY;
            public float tDeltaZ;
            public Vector3 dir;
        };

        public static RayTraceResult rayTraceSmall(Vector3 startPos, DirInfo dirInfo, RayTraceChunkManager rtm, Face faceIn, float maxLength)
        {
            int X = (int)(startPos.x + dirInfo.stepX * 0.0001f);
            int Y = (int)(startPos.y + dirInfo.stepY * 0.0001f);
            int Z = (int)(startPos.z + dirInfo.stepZ * 0.0001f);
            float tMaxX = (dirInfo.stepX > 0 ? (1 - (startPos.x - X)) : (startPos.x - X)) * dirInfo.tDeltaX;//沿射线走多远才能跳到下一个X格
            float tMaxY = (dirInfo.stepY > 0 ? (1 - (startPos.y - Y)) : (startPos.y - Y)) * dirInfo.tDeltaY;
            float tMaxZ = (dirInfo.stepZ > 0 ? (1 - (startPos.z - Z)) : (startPos.z - Z)) * dirInfo.tDeltaZ;
            int outSizeX = dirInfo.stepX > 0 ? rtm.getSizeX() : -1;
            int outSizeY = dirInfo.stepY > 0 ? rtm.getSizeY() : -1;
            int outSizeZ = dirInfo.stepZ > 0 ? rtm.getSizeZ() : -1;

            while (true) {
                if (tMaxX < tMaxY) {
                    if (tMaxX < tMaxZ) {
                        X += dirInfo.stepX;
                        if (X == outSizeX) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepX > 0 ? Face.FNI_x0 : Face.FNI_x1, tMaxX);
                        }
                        else {
                            if (tMaxX > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxX);
                            }
                            else if (rtm.testBlock(X, Y, Z)) {
                                return new RayTraceResult(true, X, Y, Z, dirInfo.stepX > 0 ? Face.FNI_x0 : Face.FNI_x1, tMaxX);
                            }
                        }
                        tMaxX += dirInfo.tDeltaX;
                    }
                    else {
                        Z += dirInfo.stepZ;
                        if (Z == outSizeZ) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepZ > 0 ? Face.FNI_z0 : Face.FNI_z1, tMaxZ);
                        }
                        else {
                            if (tMaxZ > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxZ);
                            }
                            else if (rtm.testBlock(X, Y, Z)) {
                                return new RayTraceResult(true, X, Y, Z, dirInfo.stepZ > 0 ? Face.FNI_z0 : Face.FNI_z1, tMaxZ);
                            }
                        }
                        tMaxZ += dirInfo.tDeltaZ;
                    }
                }
                else {
                    if (tMaxY < tMaxZ) {
                        Y += dirInfo.stepY;
                        if (Y == outSizeY) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepY > 0 ? Face.FNI_y0 : Face.FNI_y1, tMaxY);
                        }
                        else {
                            if (tMaxY > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxY);
                            }
                            else if (rtm.testBlock(X, Y, Z)) {
                                return new RayTraceResult(true, X, Y, Z, dirInfo.stepY > 0 ? Face.FNI_y0 : Face.FNI_y1, tMaxY);
                            }
                        }

                        tMaxY += dirInfo.tDeltaY;
                    }
                    else {
                        Z += dirInfo.stepZ;
                        if (Z == outSizeZ) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepZ > 0 ? Face.FNI_z0 : Face.FNI_z1, tMaxZ);
                        }
                        else {
                            if (tMaxZ > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxZ);
                            }
                            else if (rtm.testBlock(X, Y, Z)) {
                                return new RayTraceResult(true, X, Y, Z, dirInfo.stepZ > 0 ? Face.FNI_z0 : Face.FNI_z1, tMaxZ);
                            }
                        }

                        tMaxZ += dirInfo.tDeltaZ;
                    }
                }
            }
        }

        public static RayTraceResult rayTraceBig(Vector3 startPos, DirInfo dirInfo, RayTraceChunkManager rtm, Face faceIn, float maxLength)
        {
            int S = rtm.ChunkSize;
            int CX = (int)(startPos.x + dirInfo.stepX * 0.0001f) / S;
            int CY = (int)(startPos.y + dirInfo.stepY * 0.0001f) / S;
            int CZ = (int)(startPos.z + dirInfo.stepZ * 0.0001f) / S;

            int X = CX * S;
            int Y = CY * S;
            int Z = CZ * S;

            float curDistance = 0;

            float tMaxX = (dirInfo.stepX > 0 ? (S - (startPos.x - X)) : (startPos.x - X)) * dirInfo.tDeltaX;//沿射线走多远才能跳到下一个X格
            float tMaxY = (dirInfo.stepY > 0 ? (S - (startPos.y - Y)) : (startPos.y - Y)) * dirInfo.tDeltaY;
            float tMaxZ = (dirInfo.stepZ > 0 ? (S - (startPos.z - Z)) : (startPos.z - Z)) * dirInfo.tDeltaZ;


            int outSizeCX = dirInfo.stepX > 0 ? rtm.getSizeX() / S : -1;
            int outSizeCY = dirInfo.stepY > 0 ? rtm.getSizeY() / S : -1;
            int outSizeCZ = dirInfo.stepZ > 0 ? rtm.getSizeZ() / S : -1;

            while (true) {
                if (tMaxX < tMaxY) {
                    if (tMaxX < tMaxZ) {
                        if (!rtm.isChunkEmpty(CX, CY, CZ)) {
                            RayTraceResult smallRlt = rayTraceSmall(startPos + dirInfo.dir * curDistance, dirInfo, rtm, faceIn, Mathf.Max(tMaxX,maxLength) - curDistance);
                            if (smallRlt.bHit) {
                                smallRlt.hitLength += curDistance;
                                return smallRlt;
                            }
                        }

                        CX += dirInfo.stepX;
                        if (CX == outSizeCX) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepX > 0 ? Face.FNI_x0 : Face.FNI_x1, tMaxX);
                        }
                        else {
                            if (tMaxX > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxX);
                            }
                        }
                        curDistance = tMaxX;
                        tMaxX += dirInfo.tDeltaX * S;
                    }
                    else {
                        if (!rtm.isChunkEmpty(CX, CY, CZ)) {
                            RayTraceResult smallRlt = rayTraceSmall(startPos + dirInfo.dir * curDistance, dirInfo, rtm, faceIn, Mathf.Max(tMaxZ, maxLength) - curDistance);
                            if (smallRlt.bHit) {
                                smallRlt.hitLength += curDistance;
                                return smallRlt;
                            }
                        }

                        CZ += dirInfo.stepZ;
                        if (CZ == outSizeCZ) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepZ > 0 ? Face.FNI_z0 : Face.FNI_z1, tMaxZ);
                        }
                        else {
                            if (tMaxZ > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxZ);
                            }
                        }
                        curDistance = tMaxZ;
                        tMaxZ += dirInfo.tDeltaZ * S;
                    }
                }
                else {
                    if (tMaxY < tMaxZ) {
                        if (!rtm.isChunkEmpty(CX, CY, CZ)) {
                            RayTraceResult smallRlt = rayTraceSmall(startPos + dirInfo.dir * curDistance, dirInfo, rtm, faceIn, Mathf.Max(tMaxY, maxLength) - curDistance);
                            if (smallRlt.bHit) {
                                smallRlt.hitLength += curDistance;
                                return smallRlt;
                            }
                        }

                        CY += dirInfo.stepY;
                        if (CY == outSizeCY) {
                            return new RayTraceResult(false, X, Y, Z, dirInfo.stepY > 0 ? Face.FNI_y0 : Face.FNI_y1, tMaxY);
                        }
                        else {
                            if (tMaxY > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxY);
                            }
                        }
                        
                        curDistance = tMaxY;
                        tMaxY += dirInfo.tDeltaY * S;
                    }
                    else {
                        if (!rtm.isChunkEmpty(CX, CY, CZ)) {
                            RayTraceResult smallRlt = rayTraceSmall(startPos + dirInfo.dir * curDistance, dirInfo, rtm, faceIn, Mathf.Max(tMaxZ, maxLength) - curDistance);
                            if (smallRlt.bHit) {
                                smallRlt.hitLength += curDistance;
                                return smallRlt;
                            }
                        }

                        CZ += dirInfo.stepZ;
                        if (CZ == outSizeCZ) {
                            return new RayTraceResult(false, X, Y, X, dirInfo.stepZ > 0 ? Face.FNI_z0 : Face.FNI_z1, tMaxZ);
                        }
                        else {
                            if (tMaxZ > maxLength) {
                                return new RayTraceResult(false, X, Y, Z, Face.FNI_OutOfLength, tMaxZ);
                            }
                        }
                        
                        curDistance = tMaxZ;
                        tMaxZ += dirInfo.tDeltaZ * S;
                    }
                }
            }
        }

    }
}