#ifndef WEBRTC_AUDIO_PROCESSING_WRAPPER_HPP_
#define WEBRTC_AUDIO_PROCESSING_WRAPPER_HPP_

#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define CALL   __stdcall
#else
#define EXPORT __attribute__((visibility("default")))
#define CALL
#endif

#include <stdbool.h>
#include <stdint.h>

// AudioProcessing accepts only one of 48000, 32000, 16000, and 8000 hz.
// TODO: support multiple sample rates.
const int SAMPLE_RATE_HZ = 48000;

// AudioProcessing expects each frame to be of fixed 10 ms.
const int FRAME_MS = 10;

/// <div rustbindgen>The number of expected samples per frame.</div>
const int NUM_SAMPLES_PER_FRAME = SAMPLE_RATE_HZ * FRAME_MS / 1000;

struct AudioProcessing;

struct OptionalDouble {
    bool has_value;
    double value;
};

struct OptionalInt {
    bool has_value;
    int value;
};

struct OptionalBool {
    bool has_value;
    bool value;
};

/// <div rustbindgen>A level of noise suppression.</div>
enum NoiseSuppressionLevel {
    NOISE_SUPPRESSION_LEVEL_LOW,
    NOISE_SUPPRESSION_LEVEL_MODERATE,
    NOISE_SUPPRESSION_LEVEL_HIGH,
    NOISE_SUPPRESSION_LEVEL_VERY_HIGH,
};

/// <div rustbindgen>A level of echo suppression.</div>
enum EchoSuppressionLevel {
    ECHO_SUPPRESSION_LEVEL_LOW,
    ECHO_SUPPRESSION_LEVEL_MODERATE,
    ECHO_SUPPRESSION_LEVEL_HIGH,
};

/// <div rustbindgen>The sensitivity of the noise detector.</div>
enum VoiceDetectionLikelihood {
    VOICE_DETECTION_LIKELIHOOD_VERY_LOW,
    VOICE_DETECTION_LIKELIHOOD_LOW,
    VOICE_DETECTION_LIKELIHOOD_MODERATE,
    VOICE_DETECTION_LIKELIHOOD_HIGH,
};

/// <div rustbindgen>A configuration used only when initializing a Processor.</div>
struct InitializationConfig {
    int num_capture_channels;
    int num_render_channels;

    // TODO: Investigate how it's different from the default gain control and the effect of using the two at
    // the same time.
    bool enable_experimental_agc;

    bool enable_intelligibility_enhancer;
};

/// <div rustbindgen>Echo cancellation configuration.</div>
struct EchoCancellation {
    /// <div rustbindgen>Whether to use echo cancellation.</div>
    bool enable;

    /// <div rustbindgen>
    /// Determines the aggressiveness of the suppressor. A higher level trades off
    /// double-talk performance for increased echo suppression.
    /// </div>
    enum EchoSuppressionLevel suppression_level;

    /// <div rustbindgen>
    /// Use to enable the extended filter mode in the AEC, along with robustness
    /// measures around the reported system delays. It comes with a significant
    /// increase in AEC complexity, but is much more robust to unreliable reported
    /// delays.
    /// </div>
    bool enable_extended_filter;

    /// <div rustbindgen>
    /// Enables delay-agnostic echo cancellation. This feature relies on internally
    /// estimated delays between the process and reverse streams, thus not relying
    /// on reported system delays.
    /// </div>
    bool enable_delay_agnostic;

    /// <div rustbindgen>
    /// Sets the delay in ms between process_render_frame() receiving a far-end
    /// frame and process_capture_frame() receiving a near-end frame containing
    /// the corresponding echo. You should set this only if you are certain that
    /// the delay will be stable and constant. enable_delay_agnostic will be
    /// ignored when this option is set.
    /// </div>
    struct OptionalInt stream_delay_ms;
};

/// <div rustbindgen>Gain control configuration.</div>
struct GainControl {
    /// <div rustbindgen>Whether to use gain control.</div>
    bool enable;

    /// <div rustbindgen>Mode of gain control.</div>
    enum Mode {
        /// <div rustbindgen>Not supported yet.</div>
        /// TODO(skywhale): Expose set_stream_analog_level() and
        /// stream_analog_level().
        ADAPTIVE_ANALOG,

        /// <div rustbindgen>
        /// Bring the signal to an appropriate range by applying an adaptive gain
        /// control. The volume is dynamically amplified with a microphone with
        /// small pickup and vice versa.
        /// </div>
        ADAPTIVE_DIGITAL,

        /// <div rustbindgen>
        /// Unlike ADAPTIVE_DIGITAL, it only compresses (i.e. gradually reduces
        /// gain with increasing level) the input signal when at higher levels.
        /// Use this where the capture signal level is predictable, so that a
        /// known gain can be applied.
        /// </div>
        FIXED_DIGITAL,
    };

    /// <div rustbindgen>Determines what type of gain control is applied.</div>
    enum Mode mode;

    /// <div rustbindgen>
    /// Sets the target peak level (or envelope) of the AGC in dBFs (decibels from
    /// digital full-scale). The convention is to use positive values.
    /// For instance, passing in a value of 3 corresponds to -3 dBFs, or a target
    /// level 3 dB below full-scale. Limited to [0, 31].
    /// </div>
    int target_level_dbfs;

    /// <div rustbindgen>
    /// Sets the maximum gain the digital compression stage may apply, in dB. A
    /// higher number corresponds to greater compression, while a value of 0 will
    /// leave the signal uncompressed. Limited to [0, 90].
    /// </div>
    int compression_gain_db;

    /// <div rustbindgen>
    /// When enabled, the compression stage will hard limit the signal to the
    /// target level. Otherwise, the signal will be compressed but not limited
    /// above the target level.
    /// </div>
    bool enable_limiter;
};

/// <div rustbindgen>Noise suppression configuration.</div>
struct NoiseSuppression {
    /// <div rustbindgen>Whether to use noise supression.</div>
    bool enable;

    /// <div rustbindgen>
    /// Determines the aggressiveness of the suppression. Increasing the level will
    /// reduce the noise level at the expense of a higher speech distortion.
    /// </div>
    enum NoiseSuppressionLevel suppression_level;
};

/// <div rustbindgen>Voice detection configuration.</div>
struct VoiceDetection {
    /// <div rustbindgen>Whether to use voice detection.</div>
    bool enable;

    /// <div rustbindgen>
    /// Specifies the likelihood that a frame will be declared to contain voice. A
    /// higher value makes it more likely that speech will not be clipped, at the
    /// expense of more noise being detected as voice.
    /// </div>
    enum VoiceDetectionLikelihood detection_likelihood;
};

/// <div rustbindgen>Config that can be used mid-processing.</div>
struct WebrtcAudioProcessingConfig {
    struct EchoCancellation echo_cancellation;
    struct GainControl gain_control;
    struct NoiseSuppression noise_suppression;
    struct VoiceDetection voice_detection;

    /// <div rustbindgen>
    /// Use to enable experimental transient noise suppression.
    /// </div>
    bool enable_transient_suppressor;

    /// <div rustbindgen>
    /// Use to enable a filtering component which removes DC offset and
    /// low-frequency noise.
    /// </div>
    bool enable_high_pass_filter;
};

/// <div rustbindgen>Statistics about the processor state.</div>
struct Stats {
    /// <div rustbindgen>
    /// True if voice is detected in the current frame.
    /// </div>
    struct OptionalBool has_voice;

    /// <div rustbindgen>
    /// False if the current frame almost certainly contains no echo and true if it
    /// _might_ contain echo.
    /// </div>
    struct OptionalBool has_echo;

    /// <div rustbindgen>
    /// Root mean square (RMS) level in dBFs (decibels from digital full-scale), or
    /// alternately dBov. It is computed over all primary stream frames since the
    /// last call to |get_stats()|. The returned value is constrained to [-127, 0],
    /// where -127 indicates muted.
    /// </div>
    struct OptionalInt rms_dbfs;

    /// <div rustbindgen>
    /// Prior speech probability of the current frame averaged over output
    /// channels, internally computed by noise suppressor.
    /// </div>
    struct OptionalDouble speech_probability;

    /// <div rustbindgen>
    /// RERL = ERL + ERLE
    /// </div>
    struct OptionalDouble residual_echo_return_loss;

    /// <div rustbindgen>
    /// ERL = 10log_10(P_far / P_echo)
    /// </div>
    struct OptionalDouble echo_return_loss;

    /// <div rustbindgen>
    /// ERLE = 10log_10(P_echo / P_out)
    /// </div>
    struct OptionalDouble echo_return_loss_enhancement;

    /// <div rustbindgen>
    /// (Pre non-linear processing suppression) A_NLP = 10log_10(P_echo / P_a)
    /// </div>
    struct OptionalDouble a_nlp;

    /// <div rustbindgen>
    /// Median of the measured delay in ms. The values are aggregated until the
    /// first call to |get_stats()| and afterwards aggregated and updated every
    /// second.
    /// </div>
    struct OptionalInt delay_median_ms;

    /// <div rustbindgen>
    /// Standard deviation of the measured delay in ms. The values are aggregated
    /// until the first call to |get_stats()| and afterwards aggregated and updated
    /// every second.
    /// </div>
    struct OptionalInt delay_standard_deviation_ms;

    /// <div rustbindgen>
    /// The fraction of delay estimates that can make the echo cancellation perform
    /// poorly.
    /// </div>
    struct OptionalDouble delay_fraction_poor_delays;
};

#ifdef __cplusplus
extern "C" {
#endif

// Creates a new instance of the signal processor.
EXPORT struct AudioProcessing * CALL
webrtc_audio_processing_audio_processing_create(struct InitializationConfig init_config, int * error);

// Processes and modifies the audio frame from a capture device. Each element in
// |channels| is an array of float representing a single-channel frame of 10 ms
// length. Returns an error code or |kNoError|.
EXPORT int CALL webrtc_audio_processing_process_capture_frame(struct AudioProcessing * ap, float ** channels);

// Processes and optionally modifies the audio frame from a playback device.
// Each element in |channels| is an array of float representing a single-channel
// frame of 10 ms length. Returns an error code or |kNoError|.
EXPORT int CALL webrtc_audio_processing_process_render_frame(struct AudioProcessing * ap, float ** channel3);

// Returns statistics from the last |process_capture_frame()| call.
EXPORT struct Stats CALL webrtc_audio_processing_get_stats(struct AudioProcessing * ap);

// Immediately updates the configurations of the signal processor.
// May be called multiple times after the initialization and during processing.
EXPORT void CALL
webrtc_audio_processing_set_config(struct AudioProcessing * ap, struct WebrtcAudioProcessingConfig * config);

// Every processor created by |audio_processing_create()| needs to destroyed by
// this function.
EXPORT void CALL webrtc_audio_processing_audio_processing_delete(struct AudioProcessing * ap);

// Returns true if the code indicates a successful operation.
EXPORT bool CALL webrtc_audio_processing_is_success(int code);

#ifdef __cplusplus
}
#endif

#endif // WEBRTC_AUDIO_PROCESSING_WRAPPER_HPP_
