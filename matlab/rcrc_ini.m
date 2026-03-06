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
