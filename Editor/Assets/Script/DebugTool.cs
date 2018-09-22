using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class DebugTool2
{
    public struct Line
    {
        public Line(Vector3 start, Vector3 end, Color color)
        {
            this.start = start;
            this.end = end;
            this.color = color;
        }
        public Vector3 start;
        public Vector3 end;
        public Color color;
    }
    public List<Line> lines = new List<Line>();
    public void addLine(Vector3 start, Vector3 end, Color color)
    {
        lines.Add(new Line(start, end, color));
    }
    public static void drawLines(List<Line> lines)
    {
        foreach (Line line in lines) {
            Debug.DrawLine(line.start, line.end, line.color,0,true);
        }
    }
    public void drawAllLines()
    {
        drawLines(lines);
    }
}
