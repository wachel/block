using UnityEngine;
using System.Collections;
using System;

namespace Block
{

    public class RandomTable
    {
        public RandomTable()
        {
            tables = new Vector2[0][];
        }
        public Vector2[][] tables;//
    };

    public class RandomPoints
    {
        public RandomPoints()
        {
            randoms = new RandomTable[0];
        }
        ~RandomPoints() { }
        public Vector2[] getRandomPoints(int num)//获得一个随机的列表
        {
            if (num >= randoms.Length) {
                int oldNum = randoms.Length;
                Array.Resize(ref randoms, num + 1);
                for (int i = oldNum; i < randoms.Length; i++) {
                    randoms[i] = new RandomTable();
                }
            }
            RandomTable t = randoms[num];//因为num不会太大，所以这里为了加快速度，把num当下标使用
            if (t.tables.Length == 0) {
                t.tables = makeTables(num);	//此一次访问此num，生成
            }
            int index = UnityEngine.Random.Range(0, t.tables.Length - 1);//找到一个随机的数组
            return t.tables[index];
        }
        public static Vector3 getNewDir(float pitch, float yaw, Vector3 inDir)
        {
            Vector3 axis;
            if (Math.Abs(inDir.z) > 0.9999) {
                axis = Vector3.Cross(inDir, new Vector3(1, 0, 0));
            }
            else {
                axis = Vector3.Cross(inDir, new Vector3(0, 0, 1));
            }
            Quaternion rotPitch = Quaternion.AngleAxis(pitch * 180 / Mathf.PI, axis);
            Quaternion rotYaw = Quaternion.AngleAxis(yaw * 180 / Mathf.PI, inDir);
            return (rotYaw * rotPitch) * (inDir);
        }

        private void brownianMotion(float temperature, ref Vector3[] points)//布朗运动
        {
            //float defaultDist = Mathf.Sqrt(Mathf.PI/points.Length);
            //KDTree kdtree;
            //for(unsigned int i = 0; i<points.size(); i++){
            //    kdtree.addPoint(points[i],(void*)i);
            //}
            //kdtree.build();
            ////每个点尽量远离其他点，相当于每个点都有排斥其他点的力
            //std::vector<NodeData*>results;
            //results.resize(3);//找到离自己最近的3个点
            //for(unsigned int i = 0; i<points.size(); i++){
            //    Vector3& point = points[i];
            //    Real outMaxDist = 0;
            //    kdtree.getNearestN(point,NULL,results,outMaxDist);
            //    Vector3 force = Vector3::ZERO;	//受到的排斥力
            //    for(unsigned int r = 0; r<results.size(); r++){
            //        NodeData* node = results[r];
            //        if(int(node->pData) != i){
            //            Vector3 dir = (point - node->pos);
            //            Real lengthSq = dir.lengthSq();
            //            if(lengthSq > 0.0000000000001){
            //                dir.normalize();
            //                force += dir * defaultDist * defaultDist * 0.15f * temperature/(lengthSq);
            //            }
            //            else{
            //                force.x += defaultDist*0.001*Math::rand(-1000,1000)/1000.0;
            //                force.y += defaultDist*0.001*Math::rand(-1000,1000)/1000.0;
            //            }
            //        }
            //    }
            //    force.z = 0;
            //    if(force.length() > defaultDist*0.2){
            //        force = force*defaultDist*0.2f /force.length();
            //    }
            //    point += force;
            //    point.x += defaultDist*0.001 *Math::rand(-1000,1000)/1000.0;
            //    point.y += defaultDist*0.001 *Math::rand(-1000,1000)/1000.0;
            //    if(point.lengthSq() > 0.9999){
            //        point = point/(point.length() + defaultDist*0.5 *Math::rand(100,1000)/1000.0);
            //    }
            //}
        }
        private Vector2[][] makeTables(int num)
        {
            Vector2[][] outTables = new Vector2[10][];
            for (uint i = 0; i < outTables.Length; i++) {
                outTables[i] = buildArray(num);
            }
            return outTables;
        }
        //绕Z轴，x正方向为0
        private float getAngleFromDirection(Vector3 dir)
        {
            if (dir.x == 0) {
                if (dir.y >= 0)
                    return Mathf.PI * 0.5f;
                else
                    return Mathf.PI * 1.5f;
            }
            float angleYaw = Mathf.Atan(dir.y / dir.x);
            if (dir.x < 0) {
                angleYaw += Mathf.PI;
            }
            return angleYaw;
        }
        private Vector2[] buildArray(int num)
        {
            //首先生成圆片上随机的num个点
            System.Collections.Generic.List<Vector3> tempPoints = new System.Collections.Generic.List<Vector3>();
            while (true) {
                float x = UnityEngine.Random.Range(-100000, 100000) / 100000.0f;
                float y = UnityEngine.Random.Range(-100000, 100000) / 100000.0f;
                if (x * x + y * y < 1.0) {
                    tempPoints.Add(new Vector3(x, y, 0));
                    if (tempPoints.Count == num) {
                        break;
                    }
                }
            }

            Vector3[] points = tempPoints.ToArray();

            //将这些点做很多次布朗运动使其扩散均匀,并且逐渐降温
            for (int i = 0; i < 20; i++) {
                brownianMotion(i / 20.0f, ref points);
            }

            Vector2[] outArray = new Vector2[num];
            //利用前面步骤得到的均匀分散的点，得到Quaternion
            for (uint i = 0; i < points.Length; i++) {
                float pitchAngle = Mathf.Sin(points[i].magnitude);
                float yawAngle = getAngleFromDirection(points[i]);
                outArray[i] = new Vector2(pitchAngle, yawAngle);
            }
            return outArray;
        }
        private RandomTable[] randoms;
    };


}