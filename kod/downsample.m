[y, Fs, Nbits] = wavread('hejhej.wav');
y = y(:,1);
sec_len = length(y)/Fs;
%step = Fs/8000;
%t = 0:1/8000:sec_len;
%diff = Fs/12500;
diff = Fs/9765;
Fs = 12500;
%y = sin(2*pi*100*t);
%y2 = y;
%y2 = y(1:4:end);
%plot(y.*(abs(y)<.5))
plot(y(1:400));
%%
y = y.*(abs(y)<.5);
%wavwrite(y, 44000, 'nersamplad.wav');


%set it around 0-255
y2 = (y+abs(min(y)));
y2 = 255*y2/max(y2);

%quantize
bits = 8;
scaler = (2^(bits-1));
%y2 = round(scaler*y2)/scaler;
y2 = round(y2);
y2 = y2(1:diff:end);


plot(y2);

wavwrite((y2-mean(y2))/128, 9765, 'nersamplad.wav');
%wavplay(y2/scaler, 8000);

%y2 = y;
str = '';
for i=[1:length(y2)]
    str = strcat(str, strcat(', ', num2str(y2(i))));
    
end

L = length(y2);

NFFT = 2^nextpow2(L); % Next power of 2 from length of y
Y = fft(y2,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2+1);

% Plot single-sided amplitude spectrum.
plot(f,2*abs(Y(1:NFFT/2+1))) 
title('Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')
