function [intensity, frequency, amplitude] = tremor_analysis(hand_positions, frame_rate)
% TREMOR_ANALYSIS Analyze hand tremor from position data
%   [intensity, frequency, amplitude] = tremor_analysis(hand_positions, frame_rate)
%   where hand_positions is a Nx2 matrix of (x,y) coordinates
%   and frame_rate is the video frame rate in Hz

% Extract x and y coordinates
x = hand_positions(:,1);
y = hand_positions(:,2);

% Calculate velocity
dt = 1/frame_rate;
vx = diff(x)/dt;
vy = diff(y)/dt;
velocity = sqrt(vx.^2 + vy.^2);

% Apply bandpass filter to isolate tremor frequencies (3-12 Hz)
[b,a] = butter(4, [3 12]/(frame_rate/2), 'bandpass');
filtered_velocity = filtfilt(b, a, velocity);

% Calculate tremor intensity (RMS of filtered velocity)
intensity = rms(filtered_velocity);

% Calculate tremor frequency using FFT
n = length(filtered_velocity);
f = (0:n-1)*(frame_rate/n);
fft_velocity = abs(fft(filtered_velocity));
[~, max_idx] = max(fft_velocity(2:floor(n/2)));
frequency = f(max_idx + 1);

% Calculate tremor amplitude
amplitude = max(filtered_velocity) - min(filtered_velocity);

% Plot results
figure;
subplot(2,1,1);
plot((0:n-1)*dt, filtered_velocity);
title('Filtered Velocity');
xlabel('Time (s)');
ylabel('Velocity (pixels/s)');

subplot(2,1,2);
plot(f(1:floor(n/2)), fft_velocity(1:floor(n/2)));
title('Frequency Spectrum');
xlabel('Frequency (Hz)');
ylabel('Magnitude');

% Display results
fprintf('Tremor Analysis Results:\n');
fprintf('Intensity: %.2f pixels/s\n', intensity);
fprintf('Dominant Frequency: %.2f Hz\n', frequency);
fprintf('Amplitude: %.2f pixels/s\n', amplitude);
end 