// Simple simulation of Automatic Water Irrigation System with SMS-like control

import java.util.Scanner;

public class WaterIrrigationSystem {

    // Variables similar to Arduino
    static int moistureThreshold = 500;
    static boolean pumpManualOverride = false;
    static boolean pumpStatus = false; // false = OFF, true = ON

    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        System.out.println("Automatic Water Irrigation System Started...");
        System.out.println("Type commands: PUMP ON / PUMP OFF / STATUS / AUTO");

        while (true) {
            // Simulate reading moisture sensor value
            int moistureLevel = (int) (Math.random() * 1024); // random 0–1023 like Arduino analogRead
            System.out.println("Current Moisture Level: " + moistureLevel);

            // Check if user sends an SMS-like command
            if (System.in.available() > 0) {
                String command = input.nextLine().trim().toUpperCase();

                if (command.equals("PUMP ON")) {
                    pumpStatus = true;
                    pumpManualOverride = true;
                    System.out.println("💧 Pump turned ON manually via SMS.");
                } else if (command.equals("PUMP OFF")) {
                    pumpStatus = false;
                    pumpManualOverride = true;
                    System.out.println("🚫 Pump turned OFF manually via SMS.");
                } else if (command.equals("STATUS")) {
                    System.out.println("📱 STATUS: Moisture Level = " + moistureLevel +
                                       ", Pump = " + (pumpStatus ? "ON" : "OFF"));
                } else if (command.equals("AUTO")) {
                    pumpManualOverride = false;
                    System.out.println("🔁 Switched to automatic mode.");
                } else {
                    System.out.println("❌ Unknown command.");
                }
            }

            // Automatic control (only if not manually overridden)
            if (!pumpManualOverride) {
                if (moistureLevel < moistureThreshold && !pumpStatus) {
                    pumpStatus = true;
                    System.out.println("💦 Soil is dry. Pump turned ON automatically.");
                } else if (moistureLevel >= moistureThreshold && pumpStatus) {
                    pumpStatus = false;
                    System.out.println("🌱 Soil is moist. Pump turned OFF automatically.");
                }
            }

            // Simulate time delay (like Arduino’s delay(1000))
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
