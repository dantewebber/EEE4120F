// TSC simulation module

// set simulation time:
`timescale 1ns / 1ps

module ADC (
    input wire req,      // Request signal from TSC
    input wire rst,      // Reset signal for ADC
    output reg rdy,      // Ready signal to indicate completion
    output reg [7:0] dat // Data output from the array
);

// Constant array of values (16 data values)
  // reg [7:0]
  // integer adc_data [15:0] = {
  reg [0:7] adc_data [0:40];
    
  initial
    begin 
      // hard-coded ADC values stored in adc_data
      // it seams iVerilog doesn't want to initialize an array
      // adc_data[ 0] = 8'h8B;
      // adc_data[ 1] = 8'h8C;
      // adc_data[ 2] = 8'h99;
      // adc_data[ 3] = 8'h9B;
      // adc_data[ 4] = 8'h93;
      // adc_data[ 5] = 8'h82;
      // adc_data[ 6] = 8'h97;
      // adc_data[ 7] = 8'h90;
      // adc_data[ 8] = 8'h9F;
      // adc_data[ 9] = 8'hD7;
      // adc_data[10] = 8'h8D;
      // adc_data[11] = 8'h9C;
      // adc_data[12] = 8'h85;
      // adc_data[13] = 8'h8A;
      // adc_data[14] = 8'h91;
      // adc_data[15] = 8'h8C;

      // OVERFLOW TEST
      // adc_data[ 0] = 8'h8B;
      // adc_data[ 1] = 8'h8C;
      // adc_data[ 2] = 8'h99;
      // adc_data[ 3] = 8'h9B;
      // adc_data[ 4] = 8'h93;
      // adc_data[ 5] = 8'h82;
      // adc_data[ 6] = 8'h97;
      // adc_data[ 7] = 8'h90;
      // adc_data[ 8] = 8'h9F;
      // adc_data[ 9] = 8'h93;
      // adc_data[10] = 8'hD6;
      // adc_data[11] = 8'h9C;
      // adc_data[12] = 8'h85;
      // adc_data[13] = 8'h8A;
      // adc_data[14] = 8'h91;
      // adc_data[15] = 8'h8C;
      // adc_data[16] = 8'h81;
      // adc_data[17] = 8'h8B;
      // adc_data[18] = 8'h99;
      // adc_data[19] = 8'h9B;
      // adc_data[20] = 8'h93;
      // adc_data[21] = 8'h82;
      // adc_data[22] = 8'h97;
      // adc_data[23] = 8'h90;
      // adc_data[24] = 8'h9F;
      // adc_data[25] = 8'hA7;
      // adc_data[26] = 8'h8D;
      // adc_data[27] = 8'h9C;
      // adc_data[28] = 8'h85;
      // adc_data[29] = 8'h8A;
      // adc_data[30] = 8'h91;
      // adc_data[31] = 8'h8C;
      // adc_data[32] = 8'hD6;
      // adc_data[33] = 8'h81;
      // adc_data[34] = 8'h8B;
      // adc_data[35] = 8'h99;
      // adc_data[36] = 8'h9B;
      // adc_data[37] = 8'h93;
      // adc_data[38] = 8'h82;
      // adc_data[39] = 8'h97;
      // adc_data[40] = 8'h86;

      // Test values close to trigger value
      adc_data[ 0] = 8'hD4; // 212
      adc_data[ 1] = 8'hD4; // 212
      adc_data[ 2] = 8'hD3; // 211
      adc_data[ 3] = 8'hD2; // 210
      adc_data[ 4] = 8'h88; // 
      adc_data[ 5] = 8'hD4; // 212
      adc_data[ 6] = 8'hD4; // 212
      adc_data[ 7] = 8'hD4; // 212
      adc_data[ 8] = 8'hD4; // 212
      adc_data[ 9] = 8'hD4; // 212
      adc_data[10] = 8'hD4; // 212
      adc_data[11] = 8'hD4; // 212
      adc_data[12] = 8'hD4; // 212
      adc_data[13] = 8'hD4; // 212
      adc_data[14] = 8'hD4; // 212
      adc_data[15] = 8'hD4; // 212
      adc_data[16] = 8'hD4; // 212
      adc_data[17] = 8'hD4; // 212
      adc_data[18] = 8'hD4; // 212
      adc_data[19] = 8'hD4; // 212
      adc_data[20] = 8'hD4; // 212
      adc_data[21] = 8'hD4; // 212
      adc_data[22] = 8'hD4; // 212
      adc_data[23] = 8'hD4; // 212
      adc_data[24] = 8'hD5; // 213
      adc_data[25] = 8'hD3; // 211
      adc_data[26] = 8'hD3; // 211
      adc_data[27] = 8'hD3; // 211
      adc_data[28] = 8'hD3; // 211
      adc_data[29] = 8'hD3; // 211
      adc_data[30] = 8'hD3; // 211
      adc_data[31] = 8'hD6; // 214
      adc_data[32] = 8'hD3; // 211
      adc_data[33] = 8'hD3; // 211
      adc_data[34] = 8'hD3; // 211
      adc_data[35] = 8'hD3; // 211
      adc_data[36] = 8'hD3; // 211
      adc_data[37] = 8'hD3; // 211
      adc_data[38] = 8'hD3; // 211
      adc_data[39] = 8'hD3; // 211
      adc_data[40] = 8'hD3; // 211

    end
  
  
  reg [7:0] idx = 0; // Index to access the array
  

always @(posedge req or posedge rst) begin
    if (rst) begin
        // Reset the device
        rdy <= 0;
        dat <= 8'b00000000; // Reset data output
        idx <= 0; // Reset array index
    end else if (req) begin
        // Read data from the sample array using modular arithmetic
        dat <= adc_data[idx % 41]; // Wrap around if idx exceeds 15
        idx <= idx + 1;

        // Raise RDY line
        rdy <= 1;
    end else begin
        // Lower RDY line if not processing
        rdy <= 0;
    end
end

endmodule
