#pragma once

#include <Arduino.h>
#include <mcp2515.h>
#include <rrcan.h>
#include "Signals/Signals.h"
#include "constants.h"

#define INIT_FRAME(message) \
    can_frame message##_msg; \
    message##_msg.can_id = message##_frame_id | (message##_is_extended ? CAN_EFF_FLAG : 0); \
    message##_msg.can_dlc = message##_length

#define READ_MESSAGE_CASE(message) \
    case message##_frame_id: { \
        INIT_MESSAGE(message); \
        UNPACK_MESSAGE(message, msg.data); \
        decode_##message(message); \
        break; \
    }

class CanController {
  public:
    CanController(MCP2515& _mcp, Signals& _signals) : mcp(_mcp), signals(_signals) { }
    virtual void begin();
    virtual void update();

  private:
    MCP2515& mcp;
    Signals& signals;

    void DECODE_MESSAGE(haltech_group08);
    void DECODE_MESSAGE(haltech_group45);
    void DECODE_MESSAGE(swc_buttons);

    unsigned long lastUpdateTime = 0;
};
