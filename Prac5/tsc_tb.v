// TSC Test Bench

module tsc_tb;

    // Parameters
    parameter CLK_PERIOD = 10; // Clock period in ns

    // Signals
    reg rst, start, clk, sbf;
    wire trd, cd;
    wire [8:0] sd;

    // Instantiate the TSC module
    TSC tsc_inst (
        .rst(rst),
        .start(start),
        .clk(clk),
        .sbf(sbf),
        .trd(trd),
        .sd(sd),
        .cd(cd)
    );

    // Clock generation
    always #((CLK_PERIOD)/2) clk = ~clk;

    // Stimulus
    initial begin
        rst = 1; 
        start = 0; 
        // adc_data = 0; 
        // req = 0;
        #20 rst = 0; // Deassert reset

        // Start the TSC
        #100 start = 1;
        #100 start = 0;

        // Wait until the tigger is detected
        @(posedge trd);
        
        if (tsc_inst.trigtm == 0) begin
            $display("ERROR: TRIGTM not captured");
        end else begin
            $display("TRIGTM: %d", tsc_inst.trigtm);
        end

        // Send buffer request
        sbf = 1;
        #5 sbf = 0;

        // Wait for the data to finish sending
        @(posedge cd);

        // Display the data sent in sd
        $display("SD Data: %b", {sd[8], sd[7:0]});

        #100 $finish;
    end

endmodule