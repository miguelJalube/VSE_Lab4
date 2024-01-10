/******************************************************************************
 * (C) Copyright 2020 AMIQ Consulting
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * MODULE:      BLOG
 * PROJECT:     Non-blocking socket communication in SV using DPI-C
 * Description: This is a code snippet from the Blog article mentioned on PROJECT
 * Link:
 *******************************************************************************/
import amiq_sv_c_python_pkg::*;

module amiq_top#(int N=3, int ADDRSIZE=3, int DATASIZE=16, int ERRNO=0);

    logic clk_i = 0;
    logic rst_i;
    logic[ADDRSIZE-1:0] address_i;
    logic[1:0] byteenable_i;
    logic read_i;
    logic write_i;
    logic waitrequest_o;
    logic readdatavalid_o;
    logic[15:0] readdata_o;
    logic[15:0] writedata_i;

    default clocking cb @(posedge clk_i);
    endclocking

    always #5 clk_i = ~clk_i;

    avalon_computer#(N, ADDRSIZE, DATASIZE, ERRNO) duv(.*);


    task avalon_write(int address, int byteenable, int data);
        $display("Starting a write command");
        write_i = 1;
        address_i = address;
        writedata_i = data;
        byteenable_i = byteenable;
        @(posedge clk_i);
        while (waitrequest_o) begin
            @(posedge clk_i);
        end
        write_i = 0;
        address_i = 0;
        byteenable_i = 0;
        writedata_i = 0;
        @(negedge clk_i);
    endtask

    task avalon_read(int address, int byteenable, output int data);
        $display("Starting a read command");
        read_i = 1;
        address_i = address;
        byteenable_i = byteenable;
        @(posedge clk_i);
        while (waitrequest_o) begin
            @(posedge clk_i);
        end
        data = readdata_o;
        @(posedge clk_i);
        read_i = 0;
        address_i = 0;
        byteenable_i = 0;
        @(posedge clk_i);
    endtask

    task init_signals();
        write_i = 0;
        read_i = 0;
        address_i = 0;
        byteenable_i = 0;
        writedata_i = 0;
        @(negedge clk_i);
    endtask

    task apply_reset();
        rst_i = 1;
        #20;
        rst_i = 0;
    endtask


    initial begin
        fork
            init_signals();
            apply_reset();
        join

        @(negedge clk_i);
        @(negedge clk_i);
    end

    amiq_server_connector#(
        .hostname("127.0.0.1"),
        .port(8888),
        .delim("\n")
        ) client = new();

    function int math_compute(int a, int b, int c);
        return a * a * a + b * c;
    endfunction

    initial begin
        fork
            // Connect to server and start communication threads
            begin
                client.start();
            end


            // Recv thread:
            //      Collecting received items through the connector's recv mailbox
            begin
                string recv_msg;
                forever begin
                    client.recv_mbox.get(recv_msg);
                    $display($sformatf("Received item: {%s}", recv_msg));

                    // End of test mechanism:
                    // recognizing the end of test item as a received item
                    if(recv_msg == "end_test") begin
                        $display("End of test");
                        set_run_finish();
                        $finish();
                    end

                    begin
                        automatic string cmd_and_params;
                        automatic string parameters;
                        automatic int pos_colon;
                        automatic string cmd = "r";
                        $display("Received string: %s", recv_msg);
                        if (recv_msg.len() < 2) begin
                            continue;
                        end
                        cmd_and_params = recv_msg.substr(1, recv_msg.len() - 2);

                        for(int i = 0; i < cmd_and_params.len(); i++) begin
                            if (cmd_and_params[i] == ":") begin
                                pos_colon = i;
                                break;
                            end
                        end
                        $display($sformatf("cmd_and_params: %s", cmd_and_params));
                        cmd = cmd_and_params.substr(0, pos_colon - 1);
                        $display($sformatf("cmd: %s", cmd));
                        $display($sformatf("cmd_and_params: %s", cmd_and_params));
                        parameters = cmd_and_params.substr(pos_colon + 1, cmd_and_params.len() - 1);
                        $display($sformatf("cmd_and_params: %s", cmd_and_params));
                        $display($sformatf("parameters: %s", parameters));

                        // Find the right command
                        if (cmd == "wr") begin
                            automatic int commas[2];
                            automatic int cindex = 0;
                            automatic int addr;
                            automatic int val;
                            cindex = 0;
                            for(int i = 0; i < parameters.len(); i++) begin
                                if (parameters[i] == ",") begin
                                    commas[cindex] = i;
                                    cindex++;
                                end
                            end
                            addr = parameters.substr(0, commas[0] - 1).atoi();
                            val = parameters.substr(commas[0] + 1, parameters.len() - 1).atoi();
                            avalon_write(addr, 3, val);
                        end
                        else if (cmd == "rd") begin
                            automatic string resultString;
                            automatic int result;
                            automatic int addr;
                            addr = parameters.atoi();
                            avalon_read(addr, 3, result);
                            resultString.itoa(result);
                            client.send_mbox.put($sformatf("%s:%0d", cmd, result));
                        end
                        else if (cmd == "compute") begin
                            automatic string resultString;
                            automatic int result;
                            automatic int commas[2];
                            automatic int cindex = 0;
                            automatic int a;
                            automatic int b;
                            automatic int c;

                            $display($sformatf("parameters: %s", parameters));
                            for(int i = 0; i < parameters.len(); i++) begin
                                if (parameters[i] == ",") begin
                                    commas[cindex] = i;
                                    cindex++;
                                end
                            end
                            // Check cindex values

                            a = parameters.substr(0, commas[0] - 1).atoi();
                            b = parameters.substr(commas[0] + 1, commas[1] - 1).atoi();
                            c = parameters.substr(commas[1] + 1, parameters.len() - 1).atoi();
                            // $display($sformatf("%s:%0d,%0d,%0d", cmd_and_params, a, b, c));
                            // $display($sformatf("cmd_and_params: %s", cmd_and_params));
                            // $display($sformatf("Commas: %0d:%0d", commas[0], commas[1]));
                            // $display($sformatf("parametersc:%s", parameters.substr(commas[1] + 1, parameters.len() - 1)));
                            // $display($sformatf("parameterscs:%0d", parameters.substr(commas[1] + 1, parameters.len() - 1).len()));


                            avalon_write(0, 3, a);
                            avalon_write(1, 3, b);
                            avalon_write(2, 3, c);
                            // Start computation
                            avalon_write(4, 3, 1);
                            begin
                                automatic int status = 0;
                                while (status == 0)
                                    avalon_read(5, 3, status);
                            end
                            avalon_read(3, 3, result);

                            // In case we would like to simulate in SystemVerilog
                            // result = math_compute(a, b, c);

                            resultString.itoa(result);
                            client.send_mbox.put($sformatf("%s:%0d", cmd, result));
                        end
                    end



                end
            end
        join
    end

endmodule
