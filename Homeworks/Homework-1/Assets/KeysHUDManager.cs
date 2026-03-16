using UnityEngine;
using UnityEngine.UI;

public class KeysHUDManager : MonoBehaviour
{
    public static KeysHUDManager Instance;

    public Image[] keyImages;
    public Sprite emptyKeySprite;
    public Sprite filledKeySprite;

    private int keysCollected = 0;

    void Awake()
    {
        Instance = this;
    }

    public void CollectKey()
    {
        if (keysCollected >= keyImages.Length) return;

        keyImages[keysCollected].sprite = filledKeySprite;
        keysCollected++;
    }
}