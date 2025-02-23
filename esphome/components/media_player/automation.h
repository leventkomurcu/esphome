#pragma once

#include "esphome/core/automation.h"
#include "media_player.h"

namespace esphome {

namespace media_player {

#define MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(ACTION_CLASS, ACTION_COMMAND) \
  template<typename... Ts> class ACTION_CLASS : public Action<Ts...>, public Parented<MediaPlayer> { \
    void play(Ts... x) override { \
      this->parent_->make_call().set_command(MediaPlayerCommand::MEDIA_PLAYER_COMMAND_##ACTION_COMMAND).perform(); \
    } \
  };

#define MEDIA_PLAYER_SIMPLE_STATE_TRIGGER(TRIGGER_CLASS, TRIGGER_STATE) \
  class TRIGGER_CLASS : public Trigger<> { \
   public: \
    TRIGGER_CLASS(MediaPlayer *player) { \
      player->add_on_state_callback([this, player]() { \
        if (player->state == MediaPlayerState::MEDIA_PLAYER_STATE_##TRIGGER_STATE) \
          this->trigger(); \
      }); \
    } \
  };

MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(PlayAction, PLAY)
MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(PauseAction, PAUSE)
MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(StopAction, STOP)
MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(ToggleAction, TOGGLE)
MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(VolumeUpAction, VOLUME_UP)
MEDIA_PLAYER_SIMPLE_COMMAND_ACTION(VolumeDownAction, VOLUME_DOWN)

template<typename... Ts> class VolumeSetAction : public Action<Ts...>, public Parented<MediaPlayer> {
  TEMPLATABLE_VALUE(float, volume)
  void play(Ts... x) override { this->parent_->make_call().set_volume(this->volume_.value(x...)).perform(); }
};

class StateTrigger : public Trigger<> {
 public:
  StateTrigger(MediaPlayer *player) {
    player->add_on_state_callback([this]() { this->trigger(); });
  }
};

MEDIA_PLAYER_SIMPLE_STATE_TRIGGER(IdleTrigger, IDLE)
MEDIA_PLAYER_SIMPLE_STATE_TRIGGER(PlayTrigger, PLAYING)
MEDIA_PLAYER_SIMPLE_STATE_TRIGGER(PauseTrigger, PAUSED)

}  // namespace media_player
}  // namespace esphome
