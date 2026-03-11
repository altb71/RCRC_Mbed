clc, clear all
addpath app/
%% Open Gui

% run GPA_nucleo_UART_exported.m

% You might want to use the following code later.

% fprintf('K << %1.3ff, %1.3ff;\n', K(1), K(2));

% fprintf('A << %1.3ff, %1.3ff, %1.3ff, %1.3ff;\n', A(1,1), A(1,2), A(2,1), A(2,2));
% fprintf('B << %1.3ff, %1.3ff;\n', B(1), B(2));
% fprintf('C << %1.3ff, %1.3ff;\n', C(1,1), C(1,2));
% fprintf('H << %1.3ff, %1.3ff;\n', H(1), H(2));


%% Intro to C++ and Mbed

% Parameters
R1 = 4.7e3;  % Ohm
R2 = R1;
C1 = 470e-9; % F
C2 = C1;

% Transfer function
s = tf('s');
a = R1*R2*C1*C2;
b = R1*C1 + R1*C2 + R2*C2;
G = 1 / (a*s^2 + b*s + 1);

% --- P1, AUFGABE 1.9 ---
load G_est_00 % save G_est_00 G_est
G_500Hz_est = G_est;
load G_est_01 % save G_est_01 G_est
G_5kHz_est = G_est;
load G_est_02 % save G_est_02 G_est
G_10kHz_est = G_est;

figure(1)
bode(G_500Hz_est, G_5kHz_est, G_10kHz_est, G)
legend('Location', 'best')

% --- P1, AUFGABE 1.11 ---
load data_00.mat % save data_00 data

figure(2)
plot(data.time, data.values(:,1:3)), grid on
ylabel('Voltage (V)'), xlabel('Time (sec)')
legend('Input', ...
    'Output 1 measured', ...
    'Output 2 measured', ...
    'Location', 'best')

% --- P1, AUFGABE 1.12 ---
load data_01.mat % save data_01 data

G_cl = feedback(4 * G, 1);
y_sim = lsim(G_cl, data.values(:,4), data.time);

figure(3)
subplot(211)
plot(data.time, [data.values(:,[4 2 3]), y_sim]), grid on
ylabel('Voltage (V)')
legend('Setpoint', ...
    'Output 1 measured', ...
    'Control Output 2 measured', ...
    'Control Output 2 simulated', ...
    'Location', 'best')
subplot(212)
plot(data.time, data.values(:,1)), grid on
ylabel('Voltage (V)'), xlabel('Time (sec)')
legend('Control Input measured', ...
    'Location', 'best')

load G_est_03 % save G_est_03 G_est
G_cl_est = G_est;

figure(4)
bode(G_cl_est, G_cl), grid on
legend('Measured', ...
    'Model', ...
    'Location', 'best')
