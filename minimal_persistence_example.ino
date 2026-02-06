#include "ProfileManager.h"

// Sample profiles
profile_t local_profiles[MAX_PROFILES];
int local_count = 0;

void setup() {
  Serial.begin(115200);

  if (Profiles.begin()) {
    Serial.println("Persistence System Ready");
    Profiles.loadProfiles(local_profiles, &local_count);
  }
}

void loop() {
  // Simulating a UI action (e.g., adding a profile)
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'a') {
      Serial.println("Adding test profile...");
      strcpy(local_profiles[local_count].name, "New Profile");
      local_profiles[local_count].used = true;
      local_count++;

      // Buffers for background save (Core 0)
      Profiles.saveProfiles(local_profiles, local_count);
    }

    if (c == 's') {
      Serial.println("Force-saving now...");
      Profiles.forceSave(); // Immediate commit with bus lock
    }
  }

  delay(10);
}
