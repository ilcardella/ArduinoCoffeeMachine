#!/usr/bin/env python3

from threading import Thread
import serial
import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import copy


class SerialPlotter:
    def __init__(self, port="/dev/ttyUSB0", baudrate=9600, plot_size=1000):
        print("Starting serial connection...")
        self.plot_size = plot_size
        # Serial connection
        self.connection = serial.Serial(port, baudrate)
        # Read serial data in the background
        self.data_reader = Thread(target=self.read_serial_data)
        # Notify input data started
        self.receiving_data = False
        # Flag to stop the endless background thread
        self.active = True
        # Serial raw bytes
        self.raw_data = bytes()
        # List to store data to plot
        self.data = {
            "current": [0 for i in range(self.plot_size)],
            "target": [0 for i in range(self.plot_size)],
            "ssr": [0 for i in range(self.plot_size)],
        }
        self.previous_plot_timer = 0

    def start(self):
        print("Reading serial data...")
        # self.send("output on")
        self.data_reader.start()
        while not self.receiving_data:
            time.sleep(0.1)
        self.setup_plot()
        plt.show()

    def stop(self):
        print("Disconnecting serial connection...")
        # self.send("output off")
        self.active = False
        self.data_reader.join()
        self.connection.close()

    def setup_plot(self):
        fig = plt.figure(figsize=(10, 8))
        ax = plt.axes(xlim=(0.0, self.plot_size), ylim=(0.0, 200.0))
        ax.set_title("Gaggia PID Controller Plotter")
        ax.set_xlabel("Time")
        ax.set_ylabel("PID Controller Output")

        line_labels = ["CurrentT", "TargetT", "SSR"]
        styles = ["r-", "c-", "b-"]
        x_ax_text = ax.text(0.70, 0.95, "", transform=ax.transAxes)
        lines = []
        lines_text = []
        for i in range(len(self.data)):
            lines.append(ax.plot([], [], styles[i], label=line_labels[i])[0])
            lines_text.append(
                ax.text(0.70, 0.90 - i * 0.05, "", transform=ax.transAxes)
            )
        self.anim = animation.FuncAnimation(
            fig,
            self.decode_and_plot_data,
            fargs=(lines, lines_text, line_labels, x_ax_text),
            interval=50,
        )
        plt.legend(loc="upper left")

    def read_serial_data(self):
        time.sleep(1)
        self.connection.reset_input_buffer()
        while self.active:
            self.raw_data = self.connection.readline()
            self.receiving_data = True

    def decode_and_plot_data(self, frame, lines, lines_text, line_labels, x_ax_text):
        currentTimer = time.perf_counter()
        self.plot_timer = int((currentTimer - self.previous_plot_timer) * 1000)
        self.previous_plot_timer = currentTimer
        x_ax_text.set_text("Plot Interval = " + str(self.plot_timer) + "ms")
        # Update the plot only if new data was received
        if len(self.raw_data) < 1:
            return
        # Decode raw data
        string_data = copy.deepcopy(self.raw_data[:]).decode("utf-8").split(",")
        # Add decoded data into plot datastores
        if len(string_data) < 3:
            return
        self.data["current"].append(float(string_data[1]))
        self.data["target"].append(float(string_data[2]))
        self.data["ssr"].append(int(string_data[3]) * 10)
        # Update plot with decoded data
        lines[0].set_data(
            range(self.plot_size), self.data["current"][-self.plot_size :]
        )
        lines_text[0].set_text("[" + line_labels[0] + "] = " + str(string_data[1]))
        lines[1].set_data(range(self.plot_size), self.data["target"][-self.plot_size :])
        lines_text[1].set_text("[" + line_labels[1] + "] = " + str(string_data[2]))
        lines[2].set_data(range(self.plot_size), self.data["ssr"][-self.plot_size :])
        lines_text[2].set_text("[" + line_labels[2] + "] = " + str(string_data[3]))
        # Reset raw data
        self.raw_data = bytes()

    def send(self, string):
        print("Sending: {}".format(string))
        self.connection.write(bytes(string.encode("utf-8")))


def main():
    try:
        plotter = SerialPlotter(port="/dev/ttyUSB0", baudrate=9600)
        plotter.start()
    except serial.serialutil.SerialException as e:
        print(e)
        exit(1)
    except Exception as e:
        print(e)
    print("Exiting...")
    plotter.stop()


if __name__ == "__main__":
    main()
