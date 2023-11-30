# import wave
# import struct

# # Path to the WAV file
# wav_file = "Sound/BGM.wav"

# # Function to convert WAV file to C array
# def wav_to_c_array(wav_file, c_array_name):
#     # Open the WAV file
#     with wave.open(wav_file, 'rb') as wav:
#         # Get the sample rate of the WAV file
#         sample_rate = wav.getframerate()
#         print("Sample rate: ", sample_rate)
        
#         # Get the number of frames in the WAV file
#         num_frames = wav.getnframes()
        
#         # Read the audio data from the WAV file
#         audio_data = wav.readframes(num_frames)
        
#         # Unpack the audio data from bytes to integers
#         data = struct.unpack("<{}B".format(num_frames), audio_data)
        
#         # Create the C array with the audio data
#         c_array = "const uint8_t {}[] = {{\n    ".format(c_array_name)
#         c_array += ', '.join(str(i) for i in data)
#         c_array += "\n};\n"
    
#     # Return the C array
#     return c_array

# # Name of the C array
# c_array_name = "audio_data"

# # Convert the WAV file to C array and print it
# c_array = wav_to_c_array(wav_file, c_array_name)
# print(c_array)


import scipy.io.wavfile
import numpy as np


np.set_printoptions(threshold=np.inf)


rate, data = scipy.io.wavfile.read('Sound/final_2.wav')


print("unsigned char audioData[] = {")
for i, value in enumerate(data):
    # Print the value followed by a comma and a newline for every 10 elements
    end = ",\n" if (i + 1) % 35 == 0 else ", "
    print(f"{value}", end=end)
print("\n};")
print(len(data))
