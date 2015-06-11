arrayMoving=[
    -15753
-15679
-15917
-13293
-15856
-15605
-16029
-15512
-15297
-15668
-15770
-15701
-15615
-17669
-16021
-13908
-16106
-20001
-13382
-14396
-11740
-14057
-19048
-13926
-14984
-24767
-19962
-18609
-17955
-13885
-16419
-15833
-15992
-8580
-6822
-6952
-8589
-15845
-22186
-22501
-24344
-27641
-22384
-17080
-14057
-11928
-9869
-6219
-5153
-7856
-11797
-16831
-19410
-25040
-22728
-21947
-24505
-23345
-14552
-13545
-9231
-10357
-4664
-1482
-12913
-11832
-15181
-20605
-8215
-18115
-22792
-24745
-17523
-12824
-12227
-8506
-6933
-4535
-7631
-10120
-14462
-20148
-21826
-20478
-20804
-28325
-20091
-14544
-12361
-13202
-9854
-5632
-3041
-12280
-13861
-14661
-21709
-23036
-22920
-20426
-24239
-22401
-17767
-13341
-10133
-7425
-4924
-3502
-11840
-11661
-17948
-22401
-24516
-17701
-24929
-24392
-20018
-15535
-11324
-8098
-4106
-2008
-11343
-12423
-15131
-20918
-23258
-17781
-23595
-23868
-18366
-16166
-11155
-8309
-7093
-6151
-7260
-10444
-15000
-16287
-20766
-23985
-19760
-24897
-22405
-19040
-15205
-10349
-6972
-4477
-3349
-10232
-13044
-16074
-20888
-22427
-32760
-22556
-24681
-20582
-16899
-12357
-5814
-4212
-3783
-11180
-13053
-13617
-18347
-22583
-25295
-20848
-25878
-21206
-16513
-13158
-9850
-6553
-4787
-5382
-10692
-14097
-18952
-22917
-4175
-15915
-26504
-20684
-17670
-14328
-10282
-7656
-7087
-4629
-10360
-10453
-16177
-21094
-21948
-24602
]

arrayFlat=[
    -15836
-15662
-15482
-15846
-15654
-15745
-15787
-15786
-15752
-15675
-15814
-15664
-15658
-15758
-15774
-15828
-15679
-15800
-15729
-15845
-15611
-15768
-15757
-15728
-15791
-15704
-15793
-15812
-15853
-15744
-15804
-15508
-15747
-15736
-15688
-15731
-15792
-15806
-15721
-15787
-15679
-15804
-15757
-15707
-15672
-15777
-15944
-15646
-15818
-15781
-15901
-15694
-15669
-15729
-15661
-15738
-15824
-15858
-15578
-15918
-15720
-15770
-15833
-15908
-15681
-15880
-15772
-15789
-15736
-15816
-15785
-15408
-15837
-15752
-15729
-15833
-15748
-15856
-15737
-15733
-15722
-15764
-15805
-15832
-15701
-15822
-15781
-15747
-15781
-15722
-15786
-15737
-15861
-15785
-15777
-15846
-15580
-15750
-15787
-15693
-15460
-15678
-15748
-15712
-15824
-15825
-15918
-15656
-15716
-15708
-15672
-15717
-15776
-15619
-16008
-15742
-15814
-15777
-15720
-15728
-15744
-15892
-15942
-15633
-15890
-15624
-15724
-15758
-15844
-15526
-15698
-15798
-15784
-15742
-15729
-15822
-15750
-15749
-15818
-15757
-15451
-15901
-15731
-15801
-15777
-15884
-15761
-15634
-15760
-15573
-15886
-15701
-15716
-15653
-15789
-15812
-15811
-15782
-15785
-15686
-15512
-15909
-15711
-15812
-15764
-15677
-15989
-15586
-15806
-15653
-15748
-15791
-15842
-15760
-15732
-15745
-15772
-15845
-15727
-15662
-15468
-15840
-15781
-15793
-15766
-15820
-15713
-15748
-15829
-15588
-15669
-15826
-15646
-15785
-15781
-15741
-15605
-15786
-15826
-15716]

%%%Plotting moving and standing still vs. sample number%%%

figure(1)
plot(arrayMoving)
title('Accelerometer Z-Data while Moving');
xlabel('Sample Number');
ylabel('Accelerometer data');

figure(2)
plot(arrayFlat)
title('Accelerometer Z-Data while Moving');
xlabel('Sample Number');
ylabel('Accelerometer data');

%%%%%%MOVING DATA%%%%%%
%%%Plotting and FFT%%%
L = 200;
NFFT = 2^nextpow2(L); % Next power of 2 from length of y
Y = fft(arrayMoving,NFFT)/L;
f = Fs/2*linspace(0,1,NFFT/2+1);

%Plot single-sided amplitude spectrum
figure(3)
plot(f,2*abs(Y(1:NFFT/2+1))) 
title('Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

signal_avg = mean(arrayMoving);
fprintf('Absolute Value of Signal Average: %f\n',abs(signal_avg));
mag = abs(Y(1:NFFT/2+1));
fprintf('Magnitude of spike at 0 Hz: %f\n',mag(1));
fprintf('They are indeed equivalent!\n');

% %Frequency of a filter with coefficients b
% b = fir1(10,0.2);
% freqz(b)

FIFOarray = zeros(1,195);
for i = 1:195
    avgTemp = mean(arrayMoving(i:i+4));
    FIFOarray(i) = avgTemp;
end

figure(4)
plot(FIFOarray)

%0.5 is the Nyquist frequency, which is half of the sampling frequency;
%Nyquist is the cutoff frequency
b = fir1(10,0.2);
FIRarray = conv(b,arrayMoving);

figure(5)
plot(FIRarray)

%%%%%%NOT MOVING DATA%%%%%%
%%%Plotting and FFT%%%
Lf = 200;
NFFTf = 2^nextpow2(L); % Next power of 2 from length of y
Yf = fft(arrayFlat,NFFT)/L;
ff = Fs/2*linspace(0,1,NFFT/2+1);

%Plot single-sided amplitude spectrum
figure(6)
plot(ff,2*abs(Yf(1:NFFTf/2+1))) 
title('Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')

signal_avgf = mean(arrayFlat);
fprintf('Absolute Value of Signal Average: %f\n',abs(signal_avgf));
magf = abs(Yf(1:NFFTf/2+1));
fprintf('Magnitude of spike at 0 Hz: %f\n',magf(1));
fprintf('They are indeed equivalent!\n');

% %Frequency of a filter with coefficients b
% b = fir1(10,0.2);
% freqz(b)

FIFOarrayf = zeros(1,195);
for i = 1:195
    avgTempf = mean(arrayFlat(i:i+4));
    FIFOarrayf(i) = avgTempf;
end

figure(7)
plot(FIFOarrayf)

%0.5 is the Nyquist frequency, which is half of the sampling frequency;
%Nyquist is the cutoff frequency
b = fir1(10,0.2);
FIRarray = conv(b,arrayMoving);

figure(5)
plot(FIRarray)