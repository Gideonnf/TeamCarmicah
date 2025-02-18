using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Carmicah
{
    public static class Sound
    {
        // Sound playback with categories
        public static void PlaySFX(string soundName, float volume = -1.0f)
        {
            FunctionCalls.Sound_PlaySFX(soundName, volume);
        }

        public static void PlayBGM(string soundName, float volume = -1.0f)
        {
            FunctionCalls.Sound_PlayBGM(soundName, volume);
        }

        public static void SwitchBGM(string soundName, float volume = -1.0f)
        {
            FunctionCalls.Sound_SwitchBGM(soundName, volume);
        }

        // Volume controls
        public static void SetSoundVolume(string soundName, float volume)
        {
            FunctionCalls.Sound_SetVolume(soundName, volume);
        }

        // Sound controls
        public static void StopSoundBGM(string soundName)
        {
            FunctionCalls.Sound_StopBGM(soundName);
        }

        public static void PauseSound(string soundName)
        {
            FunctionCalls.Sound_Pause(soundName);
        }

        public static void ResumeSound(string soundName)
        {
            FunctionCalls.Sound_Resume(soundName);
        }

        // Category volume controls
        public static void SetSFXVolume(float volume)
        {
            FunctionCalls.Sound_SetCategoryVolume(0, volume); // 0 represents SFX category
        }

        public static void SetBGMVolume(float volume)
        {
            FunctionCalls.Sound_SetCategoryVolume(1, volume); // 1 represents BGM category
        }
    }
}
