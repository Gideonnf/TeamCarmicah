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
        public static void PlaySFX(string soundName, float volume = -1.0f, bool isLoop = false)
        {
            
            FunctionCalls.Sound_PlaySFX(soundName, volume, isLoop);
        }

        public static void PlayBGM(string soundName, float volume = -1.0f)
        {
            FunctionCalls.Sound_PlayBGM(soundName, volume);
        }

        public static void SwitchBGM(string soundName, float fadeTimer, float fadeDuration, bool isLoop = true, bool fadeInNext = false)
        {
            FunctionCalls.Sound_SwitchBGM(soundName, fadeTimer, fadeDuration, isLoop, fadeInNext);
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

        public static void StopSoundSFX(string soundName)
        {
            FunctionCalls.Sound_StopSFX(soundName);
        }

        public static void StopSoundSFXWithFade(string soundName, float fadeTimer, float fadeDuration)
        {
            FunctionCalls.Sound_StopSFXWithFade(soundName, fadeTimer, fadeDuration);
        }

        public static void StopSoundBGMWithFade(string soundName, float fadeTimer, float fadeDuration)
        {
            FunctionCalls.Sound_StopBGMWithFade(soundName, fadeTimer, fadeDuration);
            //CMConsole.Log("Calling");
        }

        public static void StopAllSFX()
        {
            FunctionCalls.Sound_StopAllSFX();
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
