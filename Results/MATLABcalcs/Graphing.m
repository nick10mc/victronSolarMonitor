%clear; close all; clc;

load("testDat20241107.mat");
load("testDat20241108.mat");
%load("testDat20241109.mat");

% Load data exported from Dr. Gomez's tool
load("idx.mat");
load("dti.mat");
load("p_in.mat");
load("Pw.mat");
load("Pe.mat");

% Load First Date
dateTime1 = table2array(testDat20241107(:,"Date"));
batVolts1 = table2array(testDat20241107(:,"BatVoltage"));
batAmps1 = table2array(testDat20241107(:,"BatCurrent"));
solarPow1 = table2array(testDat20241107(:,"SolarPower"));
solarYield1 = table2array(testDat20241107(:,"Yield"));
loadAmps1 = table2array(testDat20241107(:,"LoadCurrent"));

% Correct for the unsigned integer error in 11/07's graph
i=0;
b=16; % bit size
for i = 1:height(batAmps1)
    currentAmps = batAmps1(i);
    currentAmps = currentAmps/0.1;
    if (currentAmps >= 2^(b-1))
        batAmps1(i) = (currentAmps-2^b)*0.1;
    end
end


% Load second date
dateTime2 = table2array(testDat20241108(:,"Date"));
batVolts2 = table2array(testDat20241108(:,"BatVoltage"));
batAmps2 = table2array(testDat20241108(:,"BatCurrent"));
solarPow2 = table2array(testDat20241108(:,"SolarPower"));
solarYield2 = table2array(testDat20241108(:,"Yield"));
loadAmps2 = table2array(testDat20241108(:,"LoadCurrent"));

% Load Third Date
% dateTime3 = table2array(testDat20241109(:,"Date"));
% batVolts3 = table2array(testDat20241109(:,"BatVoltage"));
% batAmps3 = table2array(testDat20241109(:,"BatCurrent"));
% solarPow3 = table2array(testDat20241109(:,"SolarPower"));
% solarYield3 = table2array(testDat20241109(:,"Yield"));
% loadAmps3 = table2array(testDat20241109(:,"LoadCurrent"));

% Smooth the data out
w = 5; % Filter Window
whigh = 25; % High filter window, for power graph
batVolts1 = smoothdata(batVolts1,"rloess",w);
batAmps1 = smoothdata(batAmps1,"rloess",w);
solarPow1 = smoothdata(solarPow1,"rloess",whigh);
solarYield1 = smoothdata(solarYield1,"rloess",w);
loadAmps1 = smoothdata(loadAmps1,"rloess",w);

batVolts2 = smoothdata(batVolts2,"rloess",w);
batAmps2 = smoothdata(batAmps2,"rloess",w);
solarPow2 = smoothdata(solarPow2,"rloess",whigh);
solarYield2 = smoothdata(solarYield2,"rloess",w);
loadAmps2 = smoothdata(loadAmps2,"rloess",w);

%% Now plot the graphs
% Voltage and Bat Current
a=figure("Name","Battery Voltage and Current");

set(gca,'FontName','bahnschrift')

% Graph 1
hold on;

%subplot(2,1,1);
yyaxis left;
plot(dateTime1,batVolts1);
xlabel('Date & Time');
ylabel('Voltage (V)');
ylim([11.3,12.9]);

yyaxis right;
plot(dateTime1,batAmps1,'--');
ylabel('Current (A)')
grid on
grid minor
title("Battery Voltage and Current, Nov. 7th, 2024");
ylim([-5,1.5]);

startTime = datetime(2024,11,07,0,0,0);
endTime = datetime(2024,11,08,4,0,0);
timeVal = startTime:hours(2):endTime;
xticks(timeVal);
xtickangle(45);

legend("Voltage","Current");

hold off;

% Graph 2
b=figure("Name","Battery Voltage and Current");
set(gca,'FontName','bahnschrift')

%subplot(2,1,2);
hold on;
yyaxis left;
plot(dateTime2,batVolts2);
xlabel('Date & Time');
ylabel('Voltage (V)');
ylim([11.3,12.9]);

yyaxis right;
plot(dateTime2,batAmps2,'--');
ylabel('Current (A)')
grid on
grid minor
title("Battery Voltage and Current, Nov. 8th, 2024");
ylim([-5,1.5]);

startTime = datetime(2024,11,08,0,0,0);
endTime = datetime(2024,11,09,4,0,0);
timeVal = startTime:hours(2):endTime;
xticks(timeVal);
xtickangle(45);

legend("Voltage","Current");

hold off;

% Ignore graph 3 - No data, Battery was removed
% Graph 3
% subplot(3,1,3);
% yyaxis left;
% plot(dateTime3,batVolts3);
% xlabel('Date & Time');
% ylabel('Voltage (V)');
% 
% yyaxis right;
% plot(dateTime3,batAmps3);
% ylabel('Current (A)')
% grid on;
% grid minor;


% Yield and Solar Power Output
c=figure("Name","Total Daily Yield and Solar Power Output");
set(gca,'FontName','bahnschrift')

% Graph 1
hold on;

%subplot(2,1,1);
yyaxis left;
plot(dateTime1,solarYield1);
xlabel('Date & Time');
ylabel('Yield (KWh)');
ylim([0,0.08]);

yyaxis right;
plot(dateTime1,solarPow1,'--');
ylabel('Power (W)')
grid on
grid minor
title("Yield and Solar Power, Nov. 7th, 2024");
ylim([0,16]);

startTime = datetime(2024,11,07,0,0,0);
endTime = datetime(2024,11,08,4,0,0);
timeVal = startTime:hours(2):endTime;
xticks(timeVal);
xtickangle(45);

legend("Yield","Power");

hold off;

% Graph 2
c=figure("Name","Total Daily Yield and Solar Power Output");
set(gca,'FontName','bahnschrift')

hold on;

%subplot(2,1,2);
yyaxis left;
plot(dateTime2,solarYield2);
xlabel('Date & Time');
ylabel('Yield (KWh)');
ylim([0,0.08]);

yyaxis right;
plot(dateTime2,solarPow2,'--');
ylabel('Power (W)')
grid on
grid minor
title("Yield and Solar Power, Nov. 8th, 2024");
ylim([0,16]);

startTime = datetime(2024,11,08,0,0,0);
endTime = datetime(2024,11,09,4,0,0);
timeVal = startTime:hours(2):endTime;
xticks(timeVal);
xtickangle(45);

legend("Yield","Power");

hold off;

% Add another graph that compares the simulated and real world values
figure("Name","Simulated Vs. Real Power Values")
set(gca,'FontName','bahnschrift')
hold on;

yyaxis left;
plot(dateTime2,solarYield2,'Color',[0 0.4470 0.7410]);
xlabel('Date & Time');
ylabel('Yield (KWh)');
ylim([0,0.08]);

yyaxis right;

% Convert dti to datetime from duration
date = datetime(2024,11,8);
dt = date + dti;

plot(dt, Pe(idx,:),'Color',[0.4940 0.1840 0.5560], 'LineWidth', 2)
title({sprintf('Panel power (tilt: %.1f', p_in) + "{\circ})", sprintf('(Max: %.1f W)', Pw)})
grid on
xlabel('Date & Time')
ylabel('Power (W)')

%yyaxis right;
plot(dateTime2,solarPow2,'--','Color',[0.8500 0.3250 0.0980]);
%ylabel('Power (W)')
%grid on
grid minor
title("Simulated and Real Solar Power + Yield, Nov. 8th, 2024");

ylim([0,20]);

startTime = datetime(2024,11,08,0,0,0);
endTime = datetime(2024,11,09,4,0,0);
timeVal = startTime:hours(2):endTime;
xticks(timeVal);
xtickangle(45);

legend("Yield","Simulated: " + datestr(date),"Power Output", 'Location', 'east')

hold off;
