using Carmicah;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public class VolumeControl : Entity
    {
        public string soundType = "";
        public float volume = 100.0f;

        private float oldVol = 100.0f;

        private float volOriginal;
        private bool dragging = false;
        private Vector2 dragStartPos;
        public bool rangeGot = false;
        private float parentScaleX;

        public override void OnCreate()
        {
            Vector2 originalPos = LocalPosition;
            if (soundType == "Music")
            {
                volume = Sound.GetBGMVolume() * 100.0f;
                CMConsole.Log(volume.ToString());
                
                CMConsole.Log($"Original Music Slider : {originalPos.x.ToString()}");

            }else if(soundType == "Sound")
            {
                volume = Sound.GetSFXVolume() * 100.0f;
                CMConsole.Log(volume.ToString());
                CMConsole.Log($"Original Sound Slider : {originalPos.x.ToString()}");
            }



        }
        public override void OnUpdate(float dt)
        {
            if (!rangeGot)
            {
                Entity parent = GetParent();
                if (parent != null)
                {
                    Vector2 parentScale;
                    FunctionCalls.Transform_GetRenderingScale(parent.mID, out parentScale);
                    parentScaleX = parentScale.x * parent.Scale.x / 2.0f / 2.0f * 3.0f; // * (3/2) is for this image specifically, yes
                    rangeGot = true;
                }
            }

            if (dragging)
            {
                if (Input.IsMouseReleased(MouseButtons.MOUSE_BUTTON_LEFT))
                {
                    dragging = false;
                    CMConsole.Log(volume.ToString());
                }

                Vector2 newPos = Input.GetMouseUIPos();
                Vector2 objScale;
                FunctionCalls.Transform_GetWorldScale(mID, out objScale);
                float diffX = (newPos.x - dragStartPos.x) * objScale.x / parentScaleX * 100.0f;
                volume = volOriginal + diffX;
            }

            if (Math.Abs(volume - oldVol) > 0)
            {
                volume = Math.Min(Math.Max(0.0f, volume), 100.0f);
                Entity parent = GetParent();
                if (parent != null)
                {
                    Vector2 pos = LocalPosition;
                    Vector2 parentScale;
                    FunctionCalls.Transform_GetRenderingScale(parent.mID, out parentScale);
                    parentScaleX = parentScale.x * parent.Scale.x / 2.0f / 2.0f * 3.0f; // * (3/2) is for this image specifically, yes
                    CMConsole.Log($"Parent Scale : {parentScaleX}");
                    float percentage = (volume / 100.0f) - 0.5f;
                    CMConsole.Log($"Percentage: {percentage}");

                    pos.x = parentScaleX * percentage;
                    pos.y = 0;
                    LocalPosition = pos;
                    CMConsole.Log($"Local Position: {LocalPosition.x}");
                }
                oldVol = volume;
                switch(soundType)
                {
                    case "Music":

                        Sound.SetBGMVolume(volume / 100.0f);
                        
                        break;
                    case "Sound":
                        Sound.SetSFXVolume(volume / 100.0f);
                        break;
                }
            }
            
        }

        public override void OnMouseHover()
        {
            if (Input.IsMousePressed(MouseButtons.MOUSE_BUTTON_LEFT))
            {
                dragging = true;
                dragStartPos = Input.GetMouseUIPos();
                volOriginal = oldVol;
            }
        }
    }
}
