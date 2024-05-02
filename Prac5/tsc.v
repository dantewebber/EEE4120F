// // TSC Simulation Module

// // set simulation time:
// `timescale 1ns / 1ps

// module TSC (
//     input wire start,    // Start operation signal
//     input wire reset,    // Reset signal for TSC
//     input wire rdy,      // Ready signal from ADC
//     input wire SBF,      // Send buffer request input line
//     input reg [7:0] adc_data // Data input from ADC
//     output reg req,     // Request signal to request value from ADC
//     output reg TRD,    // Trigger detected signal
//     output reg SD,      // Serial Data output
//     output reg CD
// );

// reg [31:0] timer;
// reg [31:0] TRIGTM;
// reg [7:0] TRIGVL;
// reg [7:0] ring_buffer [31:0];

// // Assign each state a value
// parameter IDLE = 2'b00;
// parameter RUNNING = 2'b01;
// parameter TRIGGER_DETECTED = 2'b10;
// parameter SEND_BUFFER = 2'b11;

// // State register
// reg [1:0] state, nxt_state;

// // Put the machine in idle if reset is high, otherwise change state to next state
// always @(posedge clk) begin
//     if (reset) begin
//         state <= IDLE; // Put machine into idle state
//     end else begin
//         state <= nxt_state;
//     end
// end

// always @* begin
//     case(state)
//         IDLE: begin
//             if (start) begin
//                 nxt_state = RUNNING;
//             end else begin
//                 nxt_state = IDLE;
//             end
//         end
//         RUNNING: begin
//             if (adc_data > TRIGVL) begin
//                 nxt_state = TRIGGER_DETECTED;
//             end else begin
//                 nxt_state = RUNNING;
//             end
//         end
//         TRIGGER_DETECTED: begin
//         end
//         SEND_BUFFER: begin
//         end
//         default: nxt_state = IDLE;
//     endcase
// end


// always @(posedge clk) begin
//     // Update timer and capture TRIGTM on trigger
//     if (/* Trigger condition */) begin
//       timer_count <= timer_count + 1;
//       TRIGTM <= timer_count;
//     end
    
//   end



// endmodule

`timescale 1ns / 1ps

module TSC (
    input wire rst,
    input wire start,
    input wire clk,
    input wire [7:0] adc_data,
    input wire rdy,
    input wire sbf,
    output reg trd,
    output reg cd,
    // output reg [31:0] trigtm,
    output reg req,
    output reg [8:0] sd
    // output reg [31:0] timer
);

    // Instantiate ADC module
    ADC adc_inst (
        .req(req),
        .rst(rst),
        .rdy(rdy),
        .dat(adc_data)
    );

    // State definitions
    localparam IDLE         = 4'b0000;
    localparam RUNNING      = 4'b0001;
    localparam TRIGGERED    = 4'b0010;
    localparam BUFFER_SEND  = 4'b0011;

    // Constants
    localparam BUFFER_SIZE = 32;
    localparam TRIGVL = 8'b11010101; // Trigger value

    // Registers
    reg [3:0] state;
    reg [4:0] head, tail;
    reg [7:0] ring_buffer [0:BUFFER_SIZE-1];
    reg [4:0] captured_no;
    reg [7:0] i, temp;
    reg [31:0] timer;
    reg timer_increment_flag;
    output reg [31:0] trigtm;

    always @ (posedge clk or posedge rst) begin
        if (rst) begin
            // Reset values
            state <= IDLE;
            timer <= 32'h0;
            head <= 5'b0;
            tail <= 5'b0;
            trd <= 1'b0;
            cd <= 1'b1;
            sd <= 1'b0;
            captured_no <= 0;
            trigtm <= 32'h0;
            req <= 1'b0;
            i <= 8'h0;
            timer_increment_flag <= 1'b0;
            $display("TSC Reset.");
        end else begin
            // Increments timer if not in IDLE or SEND_BUFFER state
            if (timer_increment_flag) begin
                timer <= timer + 1;
            end
            // State machine
            case (state)
                IDLE: begin
                    $display("state: IDLE, sbf = %b, trd = %b", sbf, trd);
                    if (start) begin
                        trigtm <= 32'h0;
                        state <= RUNNING;
                        req <= 1'b1;
                        timer_increment_flag <= 1'b1; // Start timer
                        $display("Start received");
                    end else if (sbf && trd) begin
                        $display("Entering Buffer Send mode");
                        state <= BUFFER_SEND;
                        sd <= 1'b1;
                        cd <= 1'b0;
                    end
                end
                RUNNING: begin
                    $display("state: RUNNING.");
                    req <= 1'b1;
                    if (adc_data < TRIGVL) begin
                        if (req && rdy) begin
                            // $display("data=%d", adc_data);
                            ring_buffer[head] <= adc_data;
                            head <= (head + 1) % BUFFER_SIZE;
                            // timer <= timer + 1;
                            req <= 1'b0;
                        end
                    end else begin
                        temp <= timer;
                        state <= TRIGGERED;
                        $display("Triggered, data=%d",adc_data);
                    end
                end
                TRIGGERED: begin
                    req <= 1'b1;
                    trigtm <= temp + 1;
                    if (req && rdy) begin
                        ring_buffer[head] <= adc_data;
                        head <= (head + 1) % BUFFER_SIZE;
                        captured_no <= captured_no + 1;
                        req <= 1'b0;
                        if (captured_no == 16) begin
                            trd <= 1'b1; // pull trigger detected line high
                            req <= 1'b0; // stop adc value request
                            timer_increment_flag <= 1'b0; // stop timer
                            state <= IDLE;
                        end else begin
                            // timer <= timer + 1;
                            req <= 1'b0; // Make sure tsc isn't requesting readings from adc
                        end
                    end
                end
                BUFFER_SEND: begin
                    if (i < 32) begin
                        sd <= {1'b0, ring_buffer[tail]};
                        tail <= (tail + 1) % BUFFER_SIZE;
                        i <= i + 1;
                    end else begin
                        $display("Buffer send complete from tsc side.");
                        trd <= 1'b0;
                        sd <= 1'b0;
                        cd <= 1'b1;
                        state <= IDLE;
                    end
                end
            endcase
        end
    end

endmodule