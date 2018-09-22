using UnityEngine;
using System.Collections;
using System;

namespace Block
{
    public static class Const
    {
        public const int MaxBrightnessIteration = 256;
        public const int MaxBrightness = MaxBrightnessIteration - 1;
        public const int MaxLightIteration = 24;
        public const float BrightnessGamma = 0.935f;
        public const int ChunkSize = 32;
        public const float BlockSize = 1.0f;
        //六个面的相邻块坐标，xyz
        public static readonly int[,] AdjacencyOffset = {
        {-1,0,0},
        {1,0,0},
        {0,-1,0},
        {0,1,0},
        {0,0,-1},
        {0,0,1},};

        public static readonly VecInt3[] AdjacencyOffsetV = {
            new VecInt3(-1,0,0),
            new VecInt3(1,0,0),
            new VecInt3(0,-1,0),
            new VecInt3(0,1,0),
            new VecInt3(0,0,-1),
            new VecInt3(0,0,1),
        };

        //夹角块
        //6个面，每个面4个顶点，每个顶点有三个影响光照相邻块，每个块xyz三个坐标偏移
        public static readonly int[,,,] VertexAdjacencyOffset = 
        {
	        {//x0面
		        {{-1, 0,-1},{-1,-1,-1},{-1,-1, 0}},			//v0(0,0,0)
		        {{-1,-1, 0},{-1,-1, 1},{-1, 0, 1}},			//v1(0,0,1)
		        {{-1, 0, 1},{-1, 1, 1},{-1, 1, 0}},			//v2(0,1,1)
		        {{-1, 1, 0},{-1, 1,-1},{-1, 0,-1}}			//v3(0,1,0)
	        },
	        {//x1
		        {{1,-1,0},{1,-1,-1},{1,0,-1}},				//v0(1,0,0)
		        {{1,0,-1},{1,1,-1},{1,1,0}},					//v1(1,1,0)
		        {{1,1,0},{1,1,1},{1,0,1}},					//v2(1,1,1)
		        {{1,0,1},{1,-1,1},{1,-1,0}}					//v3(1,0,1)
	        },
	        {//y0
		        {{-1,-1,0},{-1,-1,-1},{0,-1,-1}},			//v0(0,0,0)
		        {{0,-1,-1},{1,-1,-1},{1,-1,0}},				//v0(1,0,0)
		        {{1,-1,0},{1,-1,1},{0,-1,1}},				//v0(1,0,1)
		        {{0,-1,1},{-1,-1,1},{-1,-1,0}}				//v0(0,0,1)
	        },
	        {//y1
		        {{0,1,-1},{-1,1,-1},{-1,1,0}},				//v0(0,1,0)
		        {{-1,1,0},{-1,1,1},{0,1,1}},					//v0(0,1,1)
		        {{0,1,1},{1,1,1},{1,1,0}},					//v0(1,1,1)
		        {{1,1,0},{1,1,-1},{0,1,-1}}					//v0(1,1,0)
	        },
	        {//z0
		        {{0,-1,-1},{-1,-1,-1},{-1,0,-1}},			//v0(0,0,0)
		        {{-1,0,-1},{-1,1,-1},{0,1,-1}},				//v0(0,1,0)
		        {{0,1,-1},{1,1,-1},{1,0,-1}},				//v0(1,1,0)
		        {{1,0,-1},{1,-1,-1},{0,-1,-1}}				//v0(1,0,0)
	        },
	        {//z1
		        {{-1,0,1},{-1,-1,1},{0,-1,1}},				//v0(0,0,1)
		        {{0,-1,1},{1,-1,1},{1,0,1}},					//v0(1,0,1)
		        {{1,0,1},{1,1,1},{0,1,1}},					//v0(1,1,1)
		        {{0,1,1},{-1,1,1},{-1,0,1}}					//v0(0,1,1)
	        }
        };

        //共面块
        //6个面，每个面4个顶点，每个顶点有三个共面的块，每个块xyz三个坐标偏移
        public static readonly int[,,,] VertexSharedOffset = 
        {
	        {//x0面
		        {{ 0,-1, 0},{ 0,-1,-1},{ 0, 0,-1}},		//(0,0,0)
		        {{ 0, 0, 1},{ 0,-1, 1},{ 0,-1, 0}},		//(0,0,1)
		        {{ 0, 1, 0},{ 0, 1, 1},{ 0, 0, 1}},		//(0,1,1)
		        {{ 0, 0,-1},{ 0, 1,-1},{ 0, 1, 0}},		//(0,1,0)
	        },
	        {//x1
		        {{ 0, 0, 1},{ 0,-1, 1},{ 0,-1, 0}},		//(1,0,1)
		        {{ 0,-1, 0},{ 0,-1,-1},{ 0, 0,-1}},		//(1,0,0)
		        {{ 0, 0,-1},{ 0, 1,-1},{ 0, 1, 0}},		//(1,1,0)
		        {{ 0, 1, 0},{ 0, 1, 1},{ 0, 0, 1}},		//(1,1,1)
	        },
	        {//y0
		        {{-1, 0, 0},{-1, 0,-1},{ 0, 0,-1}},		//(0,0,0)
		        {{ 0, 0,-1},{ 1, 0,-1},{ 1, 0, 0}},		//(1,0,0)
		        {{ 1, 0, 0},{ 1, 0, 1},{ 0, 0, 1}},		//(1,0,1)
		        {{ 0, 0, 1},{-1, 0, 1},{-1, 0, 0}},		//(0,0,1)
	        },
	        {//y1
		        {{ 0, 0,-1},{ 1, 0,-1},{ 1, 0, 0}},		//(1,1,0)
		        {{-1, 0, 0},{-1, 0,-1},{ 0, 0,-1}},		//(0,1,0)
		        {{ 0, 0, 1},{-1, 0, 1},{-1, 0, 0}},		//(0,1,1)
		        {{ 1, 0, 0},{ 1, 0, 1},{ 0, 0, 1}},		//(1,1,1)
	        },
	        {//z0
		        {{ 0,-1, 0},{ 1,-1, 0},{ 1, 0, 0}},		//(1,0,0)
		        {{-1, 0, 0},{-1,-1, 0},{ 0,-1, 0}},		//(0,0,0)
		        {{ 0, 1, 0},{-1, 1, 0},{-1, 0, 0}},		//(0,1,0)
		        {{ 1, 0, 0},{ 1, 1, 0},{ 0, 1, 0}},		//(1,1,0)
	        },
	        {//z1
		        {{-1, 0, 0},{-1,-1, 0},{ 0,-1, 0}},		//(0,0,1)
		        {{ 0,-1, 0},{ 1,-1, 0},{ 1, 0, 0}},		//(1,0,1)
		        {{ 1, 0, 0},{ 1, 1, 0},{ 0, 1, 0}},		//(1,1,1)
		        {{ 0, 1, 0},{-1, 1, 0},{-1, 0, 0}},		//(0,1,1)
	        }
        };

        public static readonly float[] FaceLight = {0.8f,0.8f,0.8f,0.8f,0.8f,1.0f};
        public static Vector3 getFaceNormal(int faceIndex){
	        if(faceIndex == (int)BlockFaceIndex.BFI_x0){
		        return -new Vector3(1,0,0);
	        }
	        else if(faceIndex == (int)BlockFaceIndex.BFI_x1){
		        return new Vector3(1,0,0);
	        }
	        else if(faceIndex == (int)BlockFaceIndex.BFI_y0){
		        return -new Vector3(0,1,0);
	        }
	        else if(faceIndex == (int)BlockFaceIndex.BFI_y1){
		        return new Vector3(0,1,0);
	        }
	        else if(faceIndex == (int)BlockFaceIndex.BFI_z0){
		        return -new Vector3(0,0,1);
	        }
	        else if(faceIndex == (int)BlockFaceIndex.BFI_z1){
		        return new Vector3(0,0,1);
	        }
	        return new Vector3(1,0,0);
        }
    };

    public static class BlockDebug
    {
        public static void assert(bool bValue)
        {
            if (!bValue)
            {
                throw(new System.Exception());
            }
        }
    };

    public enum BlockTypeEnum:short
    {
        Air,//默认透明块
        Out,
        Sand,//默认不透明块
    };


    public enum BlockFaceIndex:byte
    {
        BFI_x0,
        BFI_x1,
        BFI_y0,
        BFI_y1,
        BFI_z0,
        BFI_z1,
    };


    public abstract class BlockTypeFunBase
    {
        public abstract bool isVisible(short block);//可见的，用来判断是否需要生成mesh
        public abstract bool isOpacity(short block);//不透明，用来判断是否需要透光
        public abstract bool isCollider(short block);//可碰撞，用来判断是否可通过
        public abstract Rect getTextureUV(short block, BlockFaceIndex faceIndex);
        public abstract float getFaceAo(int x, int y, int z, BlockFaceIndex faceIndex);
        public abstract Color getFaceColor(short block, BlockFaceIndex faceIndex);
    };

    //public struct VecInt3
    //{
    //    public VecInt3(int x, int y, int z)
    //    {
    //        this.x = x; this.y = y; this.z = z;
    //    }
    //    public Vector3 ToVector3(){return new Vector3(x,y,z);}
    //    public int x, y, z;
    //}
    public struct BoundsInt
    {
        private bool valid;
        public VecInt3 min;
        public VecInt3 max;
        public VecInt3 Size { get { return new VecInt3(max.x - min.x + 1,max.y - min.y + 1,max.z - min.z + 1); } }
        public bool Contains(Vector3 point)
        {
            return (valid && point.x >= min.x && point.y >= min.y && point.z >= min.z && point.x <= max.x && point.y <= max.y && point.z <= max.z);
        }
        public void Encapsulate(VecInt3 point){
            if (!valid)
            {
                min = point;
                max = point;
                valid = true;
            }
            else
            {
                min.x = Mathf.Min(min.x, point.x);
                min.y = Mathf.Min(min.y, point.y);
                min.z = Mathf.Min(min.z, point.z);

                max.x = Mathf.Max(max.x, point.x);
                max.y = Mathf.Max(max.y, point.y);
                max.z = Mathf.Max(max.z, point.z);
            }
        }
        public bool IsValid() { return valid; }
    }

    

    //各个面的顶点顺序
    //x0面
    //v0(0,0,0)
    //v1(0,0,1)
    //v2(0,1,1)
    //v3(0,1,0)
    //x1
    //v0(1,0,0)
    //v1(1,1,0)
    //v2(1,1,1)
    //v3(1,0,1)
    //y0
    //v0(0,0,0)
    //v0(1,0,0)
    //v0(1,0,1)
    //v0(0,0,1)
    //y1
    //v0(0,1,0)
    //v0(0,1,1)
    //v0(1,1,1)
    //v0(1,1,0)
    //z0
    //v0(0,0,0)
    //v0(0,1,0)
    //v0(1,1,0)
    //v0(1,0,0)
    //z1
    //v0(0,0,1)
    //v0(1,0,1)
    //v0(1,1,1)
    //v0(0,1,1)

};