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
        public static void PlaySFX(uint entityID, string soundName, float volume = -1.0f, bool isLoop = false)
        {
            FunctionCalls.Sound_PlaySFX(entityID, soundName, volume, isLoop);
        }

        public static void PlayBGM(uint entityID, string soundName, float volume = -1.0f)
        {
            FunctionCalls.Sound_PlayBGM(entityID, soundName, volume);
        }

        public static void SwitchBGM(uint entityID, string soundName, float fadeTimer, float fadeDuration)
        {
            FunctionCalls.Sound_SwitchBGM(entityID, soundName, fadeTimer, fadeDuration);
        }

        // Volume controls
        public static void SetSoundVolume(string soundName, float volume)
        {
            FunctionCalls.Sound_SetVolume(soundName, volume);
        }

        // Sound controls
        public static void StopSoundBGM(uint entityID, string soundName)
        {
            FunctionCalls.Sound_StopBGM(entityID, soundName);
        }

        public static void StopSoundSFXWithFade(string soundName, float fadeTimer, float fadeDuration)
        {
            FunctionCalls.Sound_StopSFXWithFade(soundName, fadeTimer, fadeDuration);
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
