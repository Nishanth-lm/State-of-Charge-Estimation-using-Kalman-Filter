#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ALPHA 0.001
#define BETA 2.0
#define KAPPA 0

// EKF Structure
typedef struct {
    double x;  // State estimate (SoC)
    double P;  // Estimate covariance
    double Q;  // Process noise
    double R;  // Measurement noise
} EKF_t;

// UKF Structure
typedef struct {
    double x;   // State estimate (SoC)
    double P;   // Estimate covariance
    double Q;   // Process noise
    double R;   // Measurement noise
} UKF_t;

// Battery Parameters (for Li-ion)
const double Q_batt = 3600.0;  // 1Ah Battery (Coulombs)
const double dt = 1.0;         // 1 second time step
const double E0 = 3.7;         // Nominal voltage (V)
const double R0 = 0.01;        // Internal resistance (Ohms)
const double K = 0.1;          // Polarization constant
const double A = 0.3;          // Exponential factor A
const double B = 5.0;          // Exponential factor B

// Nonlinear Battery Voltage Model
double BatteryVoltageModel(double SoC, double current) {
    return E0 - (R0 * current) - (K / SoC) + (A * exp(-B * SoC));
}

// EKF Prediction Step
void EKF_Predict(EKF_t *ekf, double current) {
    ekf->x -= (current * dt / Q_batt);
    ekf->P += ekf->Q;
    if (ekf->x < 0) ekf->x = 0;
    if (ekf->x > 1) ekf->x = 1;
}

// EKF Update Step
void EKF_Update(EKF_t *ekf, double voltage_meas, double current) {
    double voltage_model = BatteryVoltageModel(ekf->x, current);
    double H = (K / (ekf->x * ekf->x)) - (A * B * exp(-B * ekf->x));

    double S = H * ekf->P * H + ekf->R;
    double K_gain = ekf->P * H / S;

    ekf->x += K_gain * (voltage_meas - voltage_model);
    ekf->P = (1 - K_gain * H) * ekf->P;
}

// UKF Sigma Points Calculation
void UKF_GenerateSigmaPoints(UKF_t *ukf, double sigma_points[3]) {
    double lambda = ALPHA * ALPHA * (1 + KAPPA) - 1;
    double sqrt_P = sqrt(ukf->P + ukf->Q);

    sigma_points[0] = ukf->x;        // Mean
    sigma_points[1] = ukf->x + sqrt_P;  // + Standard deviation
    sigma_points[2] = ukf->x - sqrt_P;  // - Standard deviation
}

// UKF Prediction Step
void UKF_Predict(UKF_t *ukf, double current) {
    double sigma_points[3];
    UKF_GenerateSigmaPoints(ukf, sigma_points);

    for (int i = 0; i < 3; i++) {
        sigma_points[i] -= (current * dt / Q_batt);
    }

    ukf->x = (sigma_points[0] + sigma_points[1] + sigma_points[2]) / 3.0;
    ukf->P = ((sigma_points[1] - ukf->x) * (sigma_points[1] - ukf->x) +
              (sigma_points[2] - ukf->x) * (sigma_points[2] - ukf->x)) / 2.0;
    ukf->P += ukf->Q;
}

// UKF Update Step
void UKF_Update(UKF_t *ukf, double voltage_meas, double current) {
    double sigma_points[3];
    UKF_GenerateSigmaPoints(ukf, sigma_points);

    double transformed_sigma[3];
    for (int i = 0; i < 3; i++) {
        transformed_sigma[i] = BatteryVoltageModel(sigma_points[i], current);
    }

    double z_mean = (transformed_sigma[0] + transformed_sigma[1] + transformed_sigma[2]) / 3.0;
    double S = ((transformed_sigma[1] - z_mean) * (transformed_sigma[1] - z_mean) +
                (transformed_sigma[2] - z_mean) * (transformed_sigma[2] - z_mean)) / 2.0;
    S += ukf->R;

    double P_xz = ((sigma_points[1] - ukf->x) * (transformed_sigma[1] - z_mean) +
                   (sigma_points[2] - ukf->x) * (transformed_sigma[2] - z_mean)) / 2.0;

    double K = P_xz / S;

    ukf->x += K * (voltage_meas - z_mean);
    ukf->P -= K * S * K;
}

int main() {
    FILE *file = fopen("D:\\RWU_E\\3rd\\SOC\\kalmanSoc\\kalmanSoc\\sensor_data.txt", "r");
    FILE *output = fopen("D:\\RWU_E\\3rd\\SOC\\kalmanSoc\\kalmanSoc\\estimated_soc.txt", "w");

    if (!file || !output) {
        printf("Error opening file!\n");
        return 1;
    }

    // Initialize EKF and UKF
    EKF_t ekf = { .x = 0.5, .P = 0.01, .Q = 1e-4, .R = 1e-2 };
    UKF_t ukf = { .x = 0.5, .P = 0.01, .Q = 1e-4, .R = 1e-2 };

    char buffer[100];
    fgets(buffer, 100, file);

    fprintf(output, "Time (s), EKF SoC, UKF SoC\n");

    int time;
    double current, voltage_meas;

    while (fscanf(file, "%d, %lf, %lf", &time, &current, &voltage_meas) == 3) {
        EKF_Predict(&ekf, current);
        EKF_Update(&ekf, voltage_meas, current);

        UKF_Predict(&ukf, current);
        UKF_Update(&ukf, voltage_meas, current);

        fprintf(output, "%d, %.4f, %.4f\n", time, ekf.x, ukf.x);
    }

    fclose(file);
    fclose(output);

    printf("EKF & UKF SoC estimation saved to 'estimated_soc.txt'\n");

    return 0;
}
