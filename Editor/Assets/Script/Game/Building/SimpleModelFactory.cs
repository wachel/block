using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public static class SimpleModelFactory 
{
    public static Dictionary<string, SimpleModel> simpleModels = new Dictionary<string,SimpleModel>();
    public static void CreateAll(){
        simpleModels["stair0"] = CreateStair(0);
        simpleModels["stair1"] = CreateStair(1);
        simpleModels["stair4"] = CreateStair(4);
        simpleModels["stair5"] = CreateStair(5);
    }

    public static SimpleModel GetModel(string name)
    {
        if (simpleModels.ContainsKey(name)) {
            return simpleModels[name];
        }
        return null;
    }

    private static SimpleModel CreateStair(int normalIndex)
    {
        SimpleModel model = new SimpleModel(2);
        switch (normalIndex) {
            case 0:
                model.FillRange(new VecInt3(0, 0, 0), new VecInt3(2, 1, 2));
                model.FillRange(new VecInt3(1, 1, 0), new VecInt3(1, 1, 2));
                break;
            case 1:
                model.FillRange(new VecInt3(0, 0, 0), new VecInt3(2, 1, 2));
                model.FillRange(new VecInt3(0, 1, 0), new VecInt3(1, 1, 2));
                break;
            case 4:
                model.FillRange(new VecInt3(0, 0, 0), new VecInt3(2, 1, 2));
                model.FillRange(new VecInt3(0, 1, 1), new VecInt3(2, 1, 1));
                break;
            case 5:
                model.FillRange(new VecInt3(0, 0, 0), new VecInt3(2, 1, 2));
                model.FillRange(new VecInt3(0, 1, 0), new VecInt3(2, 1, 1));
                break;
        }

        model.Build();
        return model;
    }


}
