#include "webrtc_audio_processing_wrapper.h"

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4100)
#endif
#include "modules/audio_processing/include/audio_processing.h"
#ifdef _WIN32
#pragma warning(pop)
#endif

#include <algorithm>
#include <memory>

namespace {

// This is the default that Chromium uses.
const int AGC_STARTUP_MIN_VOLUME = 85;

OptionalDouble make_optional_double(const double value) {
    OptionalDouble rv;
    rv.has_value = true;
    rv.value = value;
    return rv;
}

OptionalInt make_optional_int(const int value) {
    OptionalInt rv;
    rv.has_value = true;
    rv.value = value;
    return rv;
}

OptionalBool make_optional_bool(const bool value) {
    OptionalBool rv;
    rv.has_value = true;
    rv.value = value;
    return rv;
}

// static void deinterleave(const std::vector<float> & src, std::vector<std::vector<float>> * dst) {
//     const size_t num_channels = dst->size();
//     const size_t num_samples = (*dst)[0].size();
//     assert(src.size() == num_channels * num_samples);

//     for (size_t channel_index = 0; channel_index < num_channels; channel_index++) {
//         for (size_t sample_index = 0; sample_index < num_samples; sample_index++) {
//             (*dst)[channel_index][sample_index] = src[num_channels * sample_index + channel_index];
//         }
//     }
// }

// static void interleave(const std::vector<std::vector<float>> & src, std::vector<float> * dst) {
//     const size_t num_channels = src.size();
//     const size_t num_samples = src[0].size();
//     assert(dst->size() == num_channels * num_samples);

//     for (size_t channel_index = 0; channel_index < num_channels; channel_index++) {
//         for (size_t sample_index = 0; sample_index < num_samples; sample_index++) {
//             (*dst)[num_channels * sample_index + channel_index] = src[channel_index][sample_index];
//         }
//     }
// }

} // namespace

struct AudioProcessing {
    std::unique_ptr<webrtc::AudioProcessing> processor;
    webrtc::StreamConfig capture_stream_config;
    webrtc::StreamConfig render_stream_config;
    OptionalInt stream_delay_ms;
    // std::vector<std::vector<float>> capture_interleave_buffer;
    // std::vector<std::vector<float>> render_interleave_buffer;
};

AudioProcessing * webrtc_audio_processing_audio_processing_create(
    const InitializationConfig init_config, int * error) {
    AudioProcessing * ap = new AudioProcessing;
    ap->processor.reset(webrtc::AudioProcessingBuilder().Create());

    const bool has_keyboard = false;
    ap->capture_stream_config =
        webrtc::StreamConfig(SAMPLE_RATE_HZ, init_config.num_capture_channels, has_keyboard);
    ap->render_stream_config =
        webrtc::StreamConfig(SAMPLE_RATE_HZ, init_config.num_render_channels, has_keyboard);

    webrtc::ProcessingConfig pconfig = {
        ap->capture_stream_config,
        ap->capture_stream_config,
        ap->render_stream_config,
        ap->render_stream_config,
    };
    const int code = ap->processor->Initialize(pconfig);
    if (code != webrtc::AudioProcessing::kNoError) {
        *error = code;
        delete ap;
        return nullptr;
    }

    // ap->capture_interleave_buffer = std::vector<std::vector<float>>(
    //     init_config.num_capture_channels, std::vector<float>(NUM_SAMPLES_PER_FRAME, 0.0f));

    // ap->render_interleave_buffer = std::vector<std::vector<float>>(
    //     init_config.num_render_channels, std::vector<float>(NUM_SAMPLES_PER_FRAME, 0.0f));

    return ap;
}

int webrtc_audio_processing_process_capture_frame(AudioProcessing * ap, float ** channels) {
    auto * p = ap->processor.get();

    p->set_stream_delay_ms(ap->stream_delay_ms.has_value ? ap->stream_delay_ms.value : 0);

    return p->ProcessStream(channels, ap->capture_stream_config, ap->capture_stream_config, channels);
}
int webrtc_audio_processing_process_render_frame(AudioProcessing * ap, float ** channels) {
    return ap->processor->ProcessReverseStream(
        channels, ap->render_stream_config, ap->render_stream_config, channels);
}
Stats webrtc_audio_processing_get_stats(AudioProcessing * ap) {
    (void)ap;
    // TODO: add stats?
    Stats stats = {};
    return stats;
}

void webrtc_audio_processing_set_config(
    struct AudioProcessing * ap, struct WebrtcAudioProcessingConfig * config) {
    auto * p = ap->processor.get();

    webrtc::AudioProcessing::Config extra_config;

    if (config->echo_cancellation.enable) {
        ap->stream_delay_ms = config->echo_cancellation.stream_delay_ms;

        extra_config.echo_canceller.enabled = true;
        extra_config.echo_canceller.mobile_mode = false;
        extra_config.echo_canceller.enforce_high_pass_filtering = true;
    } else {
        extra_config.echo_canceller.enabled = false;
    }

    if (config->gain_control.enable) {
        extra_config.gain_controller1.enabled = true;
        // TODO: find a way to set/get the current volume and feed that with `stream_analog_level()`
        extra_config.gain_controller1.mode =
            webrtc::AudioProcessing::Config::GainController1::kAdaptiveAnalog;
        extra_config.gain_controller1.analog_level_minimum = 0;
        extra_config.gain_controller1.analog_level_maximum = 255;

        extra_config.gain_controller2.enabled = true;
    } else {
        extra_config.gain_controller1.enabled = false;
    }

    if (config->noise_suppression.enable) {
        extra_config.noise_suppression.enabled = true;
    } else {
        extra_config.noise_suppression.enabled = false;
    }

    if (config->voice_detection.enable) {
        extra_config.voice_detection.enabled = true;
    } else {
        extra_config.voice_detection.enabled = false;
    }

    if (config->enable_high_pass_filter) {
        extra_config.high_pass_filter.enabled = true;
    } else {
        extra_config.high_pass_filter.enabled = false;
    }

    extra_config.level_estimation.enabled = true;

    p->ApplyConfig(extra_config);
}

void webrtc_audio_processing_audio_processing_delete(AudioProcessing * ap) {
    delete ap;
}

bool webrtc_audio_processing_is_success(const int code) {
    return code == webrtc::AudioProcessing::kNoError;
}
