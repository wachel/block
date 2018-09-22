using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace Block
{

    public static class TerrainTool
    {
        public static DebugTool2 debugTool = new DebugTool2();
        public static void createTerrain(BlockManager blockManager, int WorldSizeX, int WorldSizeY, int WorldSizeZ)
        {
            NodeBase nodeHeight = null;
            //PerlinNoise perlin = new PerlinNoise(0);
            NodeManager nodeManager = NodeManager.load("default");
            nodeManager.forEachNodes((n) =>
            {
                if (n.node.value.getNodeType() == NodeType.HeightOutput)
                {
                    nodeHeight = n.node.value;
                }
            });
            if (nodeHeight != null)
            {
                //创建地形

                //float[,] values = nodeHeight.update(0, blockManager.baseX, blockManager.baseZ, blockManager.SizeX, blockManager.SizeZ);
                for (int x = 0; x < blockManager.SizeX; x++)
                {
                    for (int z = 0; z < blockManager.SizeZ; z++)
                    {
                        float density2d = 0.3f;// values[x, z];
                        for (int y = 0; y < blockManager.SizeY; y++)
                        {
                            //float s = 0.05f;
                            double density3d = 0;// perlin.Noise(rx * 1.23 * s, ry * 1.23 * s, rz * 1.23 * s) * 0.8;
                            if (y < WorldSizeY * Const.ChunkSize * (density2d + density3d - 0.2f) * 0.5f)
                            {
                                blockManager.setBlock(x, y, z, (short)BlockTypeEnum.Sand);
                            }
                            else
                            {
                                blockManager.setBlock(x, y, z, (short)BlockTypeEnum.Air);
                            }
                        }
                    }
                }
            }
        }
        static void addTree(BlockManager blockManager, int baseX, int baseY, int baseZ)
        {
            int treeHeight = FakeRandom.Range(10, 15);
            float radius = FakeRandom.Range(3, 6);
            for (int y = 0; y < treeHeight; y++) {
                setBlock(blockManager, baseX + 0, baseY + y, baseZ + 0, (short)Game.BlockType.Log);
            }
            for (int x = -5; x < 6; x++) {
                for (int y = 4; y < treeHeight + treeHeight/2; y++) {
                    for (int z = -5; z < 6; z++) {
                        int gx = baseX + x;
                        int gy = baseY + y;
                        int gz = baseZ + z;
                        float a = 1 - (x * x + (y - treeHeight) * (y - treeHeight) * 0.5f + z * z) / (radius * radius) - ((y - treeHeight) / (float)treeHeight) * 1.1f;
                        if (getBlock(blockManager, gx, gy, gz) == (short)BlockTypeEnum.Air) {
                            //if (a + getPerlinNoise3D(gx * 0.35, gy * 0.35, gz * 0.35) * 0.5 < 1){
                            if (a + UnityEngine.Random.Range(-0.1f, 0.1f) > 0.5f) {
                                setBlock(blockManager, gx, gy, gz, (short)Game.BlockType.Leaves);
                            }
                        }
                    }
                }
            }
        }
        public static void createTree(BlockManager blockManager, int WorldSizeX, int WorldSizeY, int WorldSizeZ)
        {
            for (int i = 0; i < WorldSizeX * Const.ChunkSize; i++)
            {
                for (int k = 0; k < WorldSizeZ * Const.ChunkSize; k++)
                {
                    bool bInSun = true;
                    for (int j = (WorldSizeY * Const.ChunkSize) - 1; j >= 0; j--)
                    {
                        if (bInSun && blockManager.getBlock(i, j, k) == (short)BlockTypeEnum.Sand)
                        {
                            if (UnityEngine.Random.Range(0, 10000) < 20)
                            {
                                addTree(blockManager, i, j, k);
                            }
                            break;
                        }
                    }
                }
            }
        }
        static void addBuilding(BlockManager blockManager, int i, int j, int k)
        {
            int dx = UnityEngine.Random.Range(8, 15);
            int dy = UnityEngine.Random.Range(8, 20);
            int dz = UnityEngine.Random.Range(8, 15);
            for (int x = -dx / 2; x < dx / 2; x++) {
                for (int y = 0; y < dy; y++) {
                    for (int z = -dz / 2; z < dz / 2; z++) {
                        int gx = x + i; int gy = y + j; int gz = z + k;
                        if (UnityEngine.Random.Range(0f, 1f) > 0.1f) {
                            setBlock(blockManager, gx, gy, gz, (short)Game.BlockType.Stone);
                        }
                    }
                }
            }
        }
        static void addHouse(BlockManager blockManager, int i, int j, int k,ref List<HouseItem>items)
        {
            FloorType[,] layout = HouseLayout.CreateHouse(HouseType.Small, (Direction)UnityEngine.Random.Range(0,4));
            FloorType[,] lineMode = LayoutToBuilding.LayoutCellToLine(layout);
            LayoutToBuilding.AddWindow(lineMode);
            Game.BlockType[, ,] blocks = LayoutToBuilding.CreateBuilding(lineMode,true);

            int dx = blocks.GetLength(0);
            int dy = blocks.GetLength(1);
            int dz = blocks.GetLength(2);

            int baseX =  i - dx/2;
            int baseY =  j;
            int baseZ =  k - dz/2;

            for (int x = 0; x < dx; x++) {
                for (int y = 0; y < dy; y++) {
                    for (int z = 0; z < dz; z++) {
                        int gx = x + baseX; int gy = y + baseY; int gz = z + baseZ;
                        if (blocks[x, y, z] != Game.BlockType.Air) {
                            setBlock(blockManager, gx, gy, gz, (short)blocks[x, y, z]);
                        }
                    }
                }
            }

            int[,] distanceToWall = LayoutToBuilding.GetDistanceToWall(lineMode);
            List<HouseItem>itemsToAdd = HouseItemGenerator.GenerateItems(lineMode, distanceToWall);
            for (int a = 0; a < itemsToAdd.Count; a++) {
                HouseItem t = itemsToAdd[a];
                items.Add(new HouseItem(new VecInt3(t.pos.x + baseX, t.pos.y + baseY, t.pos.z + baseZ), t.item));
            }
        }
        public static void createBuildings(BlockManager blockManager,ref List<HouseItem>items)
        {
            for (int i = 0; i < blockManager.SizeX; i+=30) {
                for (int k = 0; k < blockManager.SizeZ; k+=30) {
                    bool bInSun = true;
                    for (int j = (blockManager.SizeY) - 1; j >= 0; j--) {
                        if (bInSun && blockManager.getBlock(i, j, k) == (short)BlockTypeEnum.Sand) {
                            if (UnityEngine.Random.Range(0, 10000) < 3000) {
                                addHouse(blockManager, i, j + 1, k, ref items);
                            }
                            break;
                        }
                    }
                }
            }
        }

        static void setBlock(BlockManager blockManger, int posX, int posY, int posZ, short block)
        {
            //int cx = posX / Const.ChunkSize;
            //int bx = posX % Const.ChunkSize;
            //int cy = posY / Const.ChunkSize;
            //int by = posY % Const.ChunkSize;
            //int cz = posZ / Const.ChunkSize;
            //int bz = posZ % Const.ChunkSize;
            blockManger.setBlock(posX, posY, posZ, block);
        }

        static short getBlock(BlockManager blockManager, int posX, int posY, int posZ)
        {
            //int cx = posX / Const.ChunkSize;
            //int bx = posX % Const.ChunkSize;
            //int cy = posY / Const.ChunkSize;
            //int by = posY % Const.ChunkSize;
            //int cz = posZ / Const.ChunkSize;
            //int bz = posZ % Const.ChunkSize;
            //if (cx >= 0 && cx < blockManager.getSizeX() && cy >= 0 && cy < blockManager.getSizeY() && cz >= 0 && cz < blockManager.getSizeZ())
            //{
            //    return blockManager.getChunk(cx, cy, cz).getBlockSmart(bx, by, bz);
            //}
            //return (short)BlockTypeEnum.Out;
            return blockManager.getBlock(posX, posY, posZ);
        }
        
        public static Vector3[] getRandomRays(int num,int faceIndex){
            //UnityEngine.Random.seed = 12345678;
	        RandomPoints randomPoints = new RandomPoints();
	        Vector2[] angles = randomPoints.getRandomPoints(num);
	        Vector3[] rays = new Vector3[num];
	        Vector3 faceNormal = Const.getFaceNormal(faceIndex);
	        for(uint i = 0; i<angles.Length; i++){
		        rays[i] = RandomPoints.getNewDir(angles[i].x,angles[i].y,faceNormal);
	        }
            return rays;
        }

        static float calcAo(RayCastManager rayTraceManager, Vector3[] rays, int globalX, int globalY, int globalZ, int faceIndex)
        {
	        //计算ao
	        Vector3 startPos = (new Vector3(globalX,globalY,globalZ) + Vector3.one * 0.5f + Const.getFaceNormal(faceIndex) * 0.51f)*Const.BlockSize;
	        float rayLength = 16;
	        float brightness = 0.0f;
            Vector3[] startPosArray = new Vector3[rays.Length];
            float[] rayLenghtArray = new float[rays.Length];
            for (int i = 0; i < startPosArray.Length; i++) {
                startPosArray[i] = startPos;
                rayLenghtArray[i] = rayLength;
            }
            RayCastRestult[] rlts = rayTraceManager.batchRayCast(startPosArray, rays, rayLenghtArray, RayCastBlockType.Opacity);
            for (uint i = 0; i < rays.Length; i++) {
                RayCastRestult rlt = rlts[i];//rayTraceManager.rayCast(startPos, rays[i], rayLength);
                if (rlt.bHit && rlt.hitLength < rayLength) {
                    float a = ((rlt.hitLength) / rayLength);
                    brightness += (a / rays.Length);
                }
                else {
                    brightness += 1.0f / rays.Length;
                }
            }
	        brightness = Mathf.Clamp(brightness,0.0f,1.0f);
	        return Mathf.Pow(brightness,0.8f)*0.9f + 0.1f;
        }

        static float calcDirectSunLight(RayCastManager rayTraceManager, Vector3 sunDir, int globalX, int globalY, int globalZ, int faceIndex)
        {
	        //计算ao
	        Vector3 faceNormal = Const.getFaceNormal(faceIndex);
	        float faceLightFactor = Vector3.Dot(faceNormal,-sunDir);
	        if(faceLightFactor < 0){
		        return 0;
	        }
	        else{
		        Vector3 startPos = (new Vector3(globalX,globalY,globalZ) + Vector3.one * 0.5f + faceNormal * 0.501f)*Const.BlockSize;
		        float rayLength = 100.0f;
		        //float brightness = 0.0f;
		        RayCastRestult rlt = rayTraceManager.rayCast(startPos,-sunDir,rayLength,RayCastBlockType.All);
		        if(rlt.bHit){
			        return 0;
		        }
		        else{
			        return faceLightFactor;
		        }
	        }
        }

        struct BlockTestInfo
        {
            public int x;
            public int y;
            public int z;
            public int f;
            public struct RayInfo
            {
                public RayInfo(Vector3 pos, Vector3 dir, float len) {
                    this.globalPos = pos;
                    this.dir = dir;
                    this.length = len;
                }
                public Vector3 globalPos;
                public Vector3 dir;
                public float length;
            };
            public RayInfo directRay;
            public float directLight;
            public RayInfo[] aoRays;
        };

        public static void calcChunkLight(BlockChunk chunk,int cx,int cy,int cz, RayCastManager rayTraceManager,Vector3[][] rays,Vector3 sunDir)
        {
            //List<VecInt3>[] surfaces = chunk.calcSurfaceList();
            //for (int f = 0; f < 6; f++) {
            //    List<VecInt3> surs = surfaces[f];
            //    for (int i = 0; i < surs.Count; i++) {
            //        VecInt3 surface = surs[i];
            //        int gx = cx * Const.ChunkSize + surface.x;
            //        int gy = cy * Const.ChunkSize + surface.y;
            //        int gz = cz * Const.ChunkSize + surface.z;
            //        Vector3 faceNormal = Const.getFaceNormal(f);
            //        float faceLightFactor = Vector3.Dot(faceNormal, -sunDir);
            //        Vector3 startPos = (new Vector3(gx, gy, gz) + Vector3.one * 0.5f + faceNormal * 0.51f) * Const.BlockSize;
            //
            //        float directionLight = 0;
            //        float aoLight = 0;
            //        float aoLenght = 8.0f;
            //        if(faceLightFactor > 0){
            //            RayCastRestult rlt = rayTraceManager.rayCast(startPos, -sunDir, 30);
            //            directionLight += rlt.bHit ? 0 : faceLightFactor;
            //        }
            //        for (int r = 0; r < rays[f].Length; r++) {
            //            RayCastRestult rlt = rayTraceManager.rayCast(startPos, rays[f][r], aoLenght);
            //            if (rlt.bHit && rlt.hitLength < aoLenght) {
            //                float a = ((rlt.hitLength) / aoLenght);
            //                aoLight += (a / rays[f].Length);
            //            }
            //            else {
            //                aoLight += 1.0f / rays[f].Length;
            //            }
            //        }
            //        float brightness = directionLight * 0.35f + aoLight * 0.45f + 0.2f;
            //        brightness = Mathf.Clamp(brightness, 0, 1);
            //        chunk.setBrightness(surface.x, surface.y, surface.z, f, (Byte)(brightness * Const.MaxBrightness));
            //    }
            //}
        }
        public static void calcLight(BlockManager blockManager,RayCastManager rayTraceManager,Vector3[][] rays,Vector3 sunDir)
        {
            ////射线求交方式计算AO
            //blockManager.forEachChunk((BlockChunk chunk, int i, int j, int k) => {
            //    if (!chunk.isEmpty()) {
            //        calcChunkLight(chunk, i, j, k, rayTraceManager, rays, sunDir);
            //    }
            //});
        }

        enum Flag
        {
            Air = -4,          //未计算的空气
            Inner = -3,        //内部的方块
            Surface = -2,      //表面的方块
            Close = -1,        //计算过的方块
        }

        public static void calcLight2(BlockManager blockManager,int maxStep) {
            ////DebugTool.Log("开始光照计算");
            //
            //int Step = maxStep;
            //
            //List<VecInt3> surface = new List<VecInt3>();
            //const int StartLight = 0;
            //List<VecInt3>[] light = new List<VecInt3>[Step * 2 + StartLight];
            //for (int i = 0; i < light.Length; i++) {
            //    light[i] = new List<VecInt3>();
            //}
            //SByte[, ,] brightness = new SByte[blockManager.getSizeX() * Const.ChunkSize, blockManager.getSizeY() * Const.ChunkSize, blockManager.getSizeZ() * Const.ChunkSize];
            //int mbx = brightness.GetLength(0);
            //int mby = brightness.GetLength(1);
            //int mbz = brightness.GetLength(2);
            //
            //
            //for (int i = 0; i < blockManager.getSizeX(); i++) {
            //    for (int j = 0; j < blockManager.getSizeY(); j++) {
            //        for (int k = 0; k < blockManager.getSizeZ(); k++) {
            //            BlockChunk chunk = blockManager.getChunk(i, j, k);
            //            for (int x = 0; x < Const.ChunkSize; x++) {
            //                for (int y = 0; y < Const.ChunkSize; y++) {
            //                    for (int z = 0; z < Const.ChunkSize; z++) {
            //                        int gx = i * Const.ChunkSize + x;
            //                        int gy = j * Const.ChunkSize + y;
            //                        int gz = k * Const.ChunkSize + z;
            //                        short blockType = chunk.getBlock(x, y, z);
            //                        if (blockType != (short)BlockTypeEnum.Air) {
            //                            brightness[gx, gy, gz] = (SByte)Flag.Inner;
            //                        }
            //                        else {
            //                            brightness[gx, gy, gz] = (SByte)Flag.Air;
            //                        }
            //                    }
            //                }
            //            }
            //        }
            //    }
            //}
            //
            //for (int gx = 0; gx < mbx; gx++) {
            //    for (int gy = 0; gy < mby; gy++) {
            //        for (int gz = 0; gz < mbz; gz++) {
            //            if (brightness[gx, gy, gz] == (SByte)Flag.Inner) {
            //                bool bSurface = false;
            //                for (int f = 0; f < 6; f += 1) {
            //                    int adjx = gx + Const.AdjacencyOffset[f, 0];
            //                    int adjy = gy + Const.AdjacencyOffset[f, 1];
            //                    int adjz = gz + Const.AdjacencyOffset[f, 2];
            //
            //                    if (adjx >= 0 && adjx < mbx && adjy >= 0 && adjy < mby && adjz >= 0 && adjz < mbz) {
            //                        if (brightness[adjx, adjy, adjz] == (SByte)Flag.Air) {
            //                            brightness[adjx, adjy, adjz] = StartLight;
            //                            light[StartLight].Add(new VecInt3(adjx, adjy, adjz));
            //                        }
            //                        if (brightness[adjx, adjy, adjz] == StartLight) {
            //                            bSurface = true;
            //                        }
            //                    }
            //                }
            //                if (bSurface) {
            //                    brightness[gx, gy, gz] = (SByte)Flag.Surface;
            //                    surface.Add(new VecInt3(gx, gy, gz));
            //                }
            //            }
            //        }
            //    }
            //}
            //
            ////DebugTool.Log("第0步");
            ////收集亮度
            //for (int t = 0; t < Step - 1; t++) {
            //    for (int i = 0; i < light[t * 2].Count; i++) {
            //        VecInt3 v = light[t * 2][i];
            //        for (int f = 0; f < 6; f += 1) {
            //            int adjx = v.x + Const.AdjacencyOffset[f, 0];
            //            int adjy = v.y + Const.AdjacencyOffset[f, 1];
            //            int adjz = v.z + Const.AdjacencyOffset[f, 2];
            //            if (adjx >= 0 && adjx < mbx && adjy >= 0 && adjy < mby && adjz >= 0 && adjz < mbz) {
            //                if (brightness[adjx, adjy, adjz] == (short)Flag.Air) {
            //                    brightness[adjx, adjy, adjz] = (SByte)(t * 2 + 2);
            //                    light[t * 2 + 2].Add(new VecInt3(adjx, adjy, adjz));
            //                }
            //            }
            //        }
            //    }
            //}
            //
            ////DebugTool.Log("第1步");
            //
            ////亮度扩散
            //for (int t = Step * 2 - 1; t > 0; t--) {
            //    for (int i = 0; i < light[t].Count; i++) {
            //        VecInt3 v = light[t][i];
            //        SByte targetBrightness = (SByte)(t - 1);
            //        for (int f = 0; f < 6; f += 1) {
            //            int adjx = v.x + Const.AdjacencyOffset[f, 0];
            //            int adjy = v.y + Const.AdjacencyOffset[f, 1];
            //            int adjz = v.z + Const.AdjacencyOffset[f, 2];
            //            if (adjx >= 0 && adjx < mbx && adjy >= 0 && adjy < mby && adjz >= 0 && adjz < mbz) {
            //                if (brightness[adjx, adjy, adjz] < targetBrightness && brightness[adjx, adjy, adjz] >= 0) {
            //                    brightness[adjx, adjy, adjz] = targetBrightness;
            //                    light[targetBrightness].Add(new VecInt3(adjx, adjy, adjz));
            //                }
            //            }
            //        }
            //    }
            //}
            //
            //
            ////DebugTool.Log("第2步");
            //
            ////for (int i = 0; i < brightness.GetLength(0); i++) {
            ////    for (int j = 0; j < brightness.GetLength(1); j++) {
            ////        for (int k = 0; k < brightness.GetLength(2); k++) {
            ////            if (brightness[i, j, k] == Step * 2 - 6) {
            ////                debugTool.addLine(new Vector3(i, j, k), new Vector3(i, j, k) + Vector3.one * 0.5f, Color.red);;
            ////            }
            ////        }
            ////    }
            ////}
            //
            ////应用亮度
            //for (int a = 0; a < surface.Count; a++) {
            //    VecInt3 v = surface[a];
            //    int i = v.x / Const.ChunkSize;
            //    int j = v.y / Const.ChunkSize;
            //    int k = v.z / Const.ChunkSize;
            //    int x = v.x % Const.ChunkSize;
            //    int y = v.y % Const.ChunkSize;
            //    int z = v.z % Const.ChunkSize;
            //    BlockChunk chunk = blockManager.getChunk(i, j, k);
            //    SByte curValue = brightness[v.x, v.y, v.z];
            //    for (int f = 0; f < 6; f++) {
            //        int adjx = v.x + Const.AdjacencyOffset[f, 0];
            //        int adjy = v.y + Const.AdjacencyOffset[f, 1];
            //        int adjz = v.z + Const.AdjacencyOffset[f, 2];
            //        int ladjx = x + Const.AdjacencyOffset[f, 0];
            //        int ladjy = y + Const.AdjacencyOffset[f, 1];
            //        int ladjz = z + Const.AdjacencyOffset[f, 2];
            //
            //        if (adjx >= 0 && adjx < mbx && adjy >= 0 && adjy < mby && adjz >= 0 && adjz < mbz) {
            //            SByte adjValue = brightness[adjx, adjy, adjz];
            //            if (adjValue >= 0) {
            //                if (adjValue == 0) {
            //                    int ccc = 0;
            //                }
            //                float l = Mathf.Clamp((float)adjValue / (Step), 0, 1);
            //                //l = Mathf.Pow(l, 0.8f);
            //                l = Mathf.Lerp(0.0f, 1, l);
            //                Vector3 faceNormal = Const.getFaceNormal(f);
            //                float faceLightFactor = Vector3.Dot(faceNormal, Vector3.up);
            //                float directLight = faceLightFactor;
            //                l = l * 0.9f + directLight * 0.1f;
            //                l = Mathf.Clamp(l,0,0.999f);
            //                byte bright = (byte)(Const.MaxBrightness * l);
            //                chunk.setBrightness(x, y, z, f, (byte)bright);
            //            }
            //        }
            //    }
            //}
            //
            ////DebugTool.Log("计算完成");
            //
        }
    }
};