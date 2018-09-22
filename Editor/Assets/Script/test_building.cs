using UnityEngine;
using System.Collections;

public class test_building : MonoBehaviour {

	// Use this for initialization
	void Start () {
        float startTime = Time.realtimeSinceStartup;
        for (int i = 0; i < 10; i++) {
            FloorType[,] layout = HouseLayout.CreateHouse(HouseType.Small,Direction.Down);
            FloorType[,] lineMode = LayoutToBuilding.LayoutCellToLine(layout);
            LayoutToBuilding.AddWindow(lineMode);
            int[,] distanceToWall = LayoutToBuilding.GetDistanceToWall(lineMode);

            //Game.BlockType[,,] blocks = LayoutToBuilding.CreateBuilding(lineMode,true);

            string s = BuildingDebugTools.GetLayoutString(lineMode);
            Debug.Log(s);

            string dtw = Array2DTool.GetDebugStringFormat(distanceToWall, "{0:00}", ",");
            Debug.Log(dtw);
        }
        Debug.Log("time = " + (Time.realtimeSinceStartup - startTime));
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
