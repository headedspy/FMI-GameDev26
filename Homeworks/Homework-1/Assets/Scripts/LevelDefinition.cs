using System;
using System.Collections.Generic;
using UnityEngine;

public enum TileType { Normal, DangerSpikes, DangerEnemy, Damaging, LightningPickup, Key }

[Serializable]
public class LevelSegment
{
    public TileType tileType;
    [Min(1)] public int count = 1;
}

[CreateAssetMenu(fileName = "LevelDefinition", menuName = "Level/Definition")]
public class LevelDefinition : ScriptableObject
{
    public List<LevelSegment> segments = new();

    public List<TileType> BuildSequence()
    {
        var sequence = new List<TileType>();
        foreach (var segment in segments)
            for (int i = 0; i < segment.count; i++)
                sequence.Add(segment.tileType);
        return sequence;
    }
}