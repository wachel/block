using UnityEngine;
using System.Collections;

public class Misc
{
    static public GameObject findChildObject(GameObject fromGameObject, string withName) {
        Transform[] ts = fromGameObject.transform.GetComponentsInChildren<Transform>(true);
        foreach (Transform t in ts) {
            if (t.gameObject.name == withName) {
                return t.gameObject;
            }
        }
        return null;
    }
    public static bool rayHitAABB(Vector3 start, Vector3 dir, Vector3 aabbMin, Vector3 aabbMax, ref float hitTime) {
        // r.dir is unit direction vector of ray
        Vector3 dirfrac = new Vector3();
        dirfrac.x = 1.0f / dir.x;
        dirfrac.y = 1.0f / dir.y;
        dirfrac.z = 1.0f / dir.z;
        // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
        // r.org is origin of ray
        float t1 = (aabbMin.x - start.x) * dirfrac.x;
        float t2 = (aabbMax.x - start.x) * dirfrac.x;
        float t3 = (aabbMin.y - start.y) * dirfrac.y;
        float t4 = (aabbMax.y - start.y) * dirfrac.y;
        float t5 = (aabbMin.z - start.z) * dirfrac.z;
        float t6 = (aabbMax.z - start.z) * dirfrac.z;
        float tmin = Mathf.Max(Mathf.Max(Mathf.Min(t1, t2), Mathf.Min(t3, t4)), Mathf.Min(t5, t6));
        float tmax = Mathf.Min(Mathf.Min(Mathf.Max(t1, t2), Mathf.Max(t3, t4)), Mathf.Max(t5, t6));

        // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
        if (tmax < 0) {
            hitTime = tmax;
            return true;
        }

        // if tmin > tmax, ray doesn't intersect AABB
        if (tmin > tmax) {
            hitTime = tmax;
            return false;
        }

        hitTime = tmin;
        return true;
    }
    static Color32 uintToColor32(uint argb) {
        byte a = (byte)(argb >> 24);
        byte r = (byte)(((argb & 0xff0000) >> 16));
        byte g = (byte)(((argb & 0xff00) >> 8));
        byte b = (byte)((argb & 0xff));
        return new Color32(r, g, b, a);
    }
}
