#include "ProfileManager.h"

// Define these in your display driver port
extern "C" bool lvgl_port_stop_render(int timeout_ms);
extern "C" void lvgl_port_resume_render(void);

ProfileManager Profiles;

ProfileManager::ProfileManager()
    : _lock(NULL), _saveTaskHandle(NULL), _dirty(false), _lastChangeTime(0),
      _cachedCount(0), _cacheLoaded(false), isInitialized(false) {}

bool ProfileManager::begin() {
  if (isInitialized)
    return true;

  _lock = xSemaphoreCreateRecursiveMutex();
  if (_lock == NULL)
    return false;

  // Verify NVS
  Preferences p;
  if (!p.begin(NVS_NAMESPACE, false))
    return false;
  p.end();

  xTaskCreatePinnedToCore(saveTaskWorker, "NVS_Task", 4096, this, 1,
                          &_saveTaskHandle, 0);

  isInitialized = true;
  return true;
}

void ProfileManager::saveTaskWorker(void *param) {
  ProfileManager *instance = (ProfileManager *)param;
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(500));

    bool needsSave = false;
    if (xSemaphoreTakeRecursive(instance->_lock, 10) == pdTRUE) {
      if (instance->_dirty && (millis() - instance->_lastChangeTime > 3000)) {
        needsSave = true;
      }
      xSemaphoreGiveRecursive(instance->_lock);
    }

    if (needsSave) {
      // THE "ZERO-TEAR" CORE LOGIC
      if (lvgl_port_stop_render(1000)) {
        xSemaphoreTakeRecursive(instance->_lock, portMAX_DELAY);
        if (instance->saveToNVS(instance->_cachedProfiles,
                                instance->_cachedCount)) {
          instance->_dirty = false;
        }
        xSemaphoreGiveRecursive(instance->_lock);
        lvgl_port_resume_render();
      }
    }
  }
}

bool ProfileManager::saveToNVS(const profile_t *profiles, int count) {
  Preferences p;
  if (!p.begin(NVS_NAMESPACE, false))
    return false;

  if (count > 0) {
    p.putBytes(NVS_KEY_BLOB, profiles, count * sizeof(profile_t));
  }
  p.putInt(NVS_KEY_COUNT, count);
  p.putInt(NVS_KEY_VERSION, NVS_VERSION_VAL);
  p.end(); // Flush to flash
  return true;
}

bool ProfileManager::loadProfiles(profile_t *profiles, int *count) {
  xSemaphoreTakeRecursive(_lock, portMAX_DELAY);

  Preferences p;
  if (!p.begin(NVS_NAMESPACE, true)) {
    *count = 0;
    xSemaphoreGiveRecursive(_lock);
    return true;
  }

  int stored = p.getInt(NVS_KEY_COUNT, 0);
  if (stored > 0) {
    p.getBytes(NVS_KEY_BLOB, _cachedProfiles, stored * sizeof(profile_t));
  }
  _cachedCount = stored;
  _cacheLoaded = true;
  p.end();

  memcpy(profiles, _cachedProfiles, stored * sizeof(profile_t));
  *count = stored;

  xSemaphoreGiveRecursive(_lock);
  return true;
}

bool ProfileManager::saveProfiles(const profile_t *profiles, int count) {
  xSemaphoreTakeRecursive(_lock, portMAX_DELAY);
  memcpy(_cachedProfiles, profiles, count * sizeof(profile_t));
  _cachedCount = count;
  _dirty = true;
  _lastChangeTime = millis();
  xSemaphoreGiveRecursive(_lock);
  return true;
}

bool ProfileManager::forceSave() {
  xSemaphoreTakeRecursive(_lock, portMAX_DELAY);
  bool needsSave = _dirty;
  xSemaphoreGiveRecursive(_lock);

  if (needsSave) {
    if (lvgl_port_stop_render(1000)) {
      xSemaphoreTakeRecursive(_lock, portMAX_DELAY);
      saveToNVS(_cachedProfiles, _cachedCount);
      _dirty = false;
      xSemaphoreGiveRecursive(_lock);
      lvgl_port_resume_render();
      return true;
    }
  }
  return false;
}
