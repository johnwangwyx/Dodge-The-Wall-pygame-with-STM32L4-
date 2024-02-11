import scipy.io.wavfile
import numpy as np

rate, data = scipy.io.wavfile.read('sound sample.wav')
sin_data = np.sin(data)

print(sin_data)