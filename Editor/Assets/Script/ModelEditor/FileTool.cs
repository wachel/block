using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Serialization;

public class FileTool
{
    public static string[] ListDir(string path)
    {
        return Directory.GetDirectories(Application.persistentDataPath + "/" + path);
    }
    public static string[] ListFile(string path)
    {
        return Directory.GetFiles(Application.persistentDataPath + "/" + path);
    }
    public static void Save<T>(T obj, string path,bool isBinary = true)
    {
        if (isBinary) {
            FileStream stream = new FileStream(Application.persistentDataPath + "/" + path, FileMode.OpenOrCreate);
            BinaryFormatter bFormatter = new BinaryFormatter();
            bFormatter.Serialize(stream, obj);
            stream.Close();
        }
        else {
            XmlSerializer serializer = new XmlSerializer(typeof(T));
            TextWriter textWriter = new StreamWriter(Application.persistentDataPath + "/" + path);
            serializer.Serialize(textWriter, obj);
            textWriter.Close();
        }
    }
    public static object Read(string path,bool isBinary = false)
    {
        FileStream stream = new FileStream(Application.persistentDataPath + "/" + path, FileMode.Open);
        BinaryFormatter bFormatter = new BinaryFormatter();
        object result = bFormatter.Deserialize(stream);
        stream.Close();
        return result;
    }
}
