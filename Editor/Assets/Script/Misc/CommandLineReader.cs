#region Using
using System;
using System.Collections.Generic;
#endregion

public class CommandLineReader
{
    private static Dictionary<string, string> values = new Dictionary<string,string>();
    public static void parse(string cmdLine)
    {
        string[] rlt = cmdLine.Split(' ');
        string curReadingKey = "";
        for (int i = 0; i < rlt.Length; i++) {

            if (rlt[i].StartsWith("-")) {
                if (curReadingKey != "") {
                    values[curReadingKey] = "";
                }
                curReadingKey = rlt[i].Substring(1);
            }
            else {
                values[curReadingKey] = rlt[i];
                curReadingKey = "";
            }
        }
        if (curReadingKey != "") {
            values[curReadingKey] = "";
        }
    }
    public static string getValue(string optionName)
    {
        if (values.ContainsKey(optionName)) {
            return values[optionName];
        }
        return "";
    }
    public static bool hasOption(string optionName)
    {
        return values.ContainsKey(optionName);
    }
}