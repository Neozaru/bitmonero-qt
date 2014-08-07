package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"
)

type GetInfoReply struct {
	Status                     string `json:"status"`
	Height                     int    `json:"height"`
	Target_height              int    `json:"target_height"`
	Difficulty                 int    `json:"difficulty"`
	Tx_count                   int    `json:"tx_count"`
	Tx_pool_size               int    `json:"tx_pool_size"`
	Alt_blocks_count           int    `json:"alt_blocks_count"`
	Outgoing_connections_count int    `json:"outgoing_connections_count"`
	Incoming_connections_count int    `json:"incoming_connections_count"`
	White_peerlist_size        int    `json:"white_peerlist_size"`
	Grey_peerlist_size         int    `json:"grey_peerlist_size"`
}

func main() {

	http.HandleFunc("/getinfo", func(w http.ResponseWriter, r *http.Request) {
		reply := GetInfoReply{
			Height:                     4000,
			Target_height:              4200,
			Difficulty:                 25,
			Tx_count:                   420,
			Tx_pool_size:               420,
			Alt_blocks_count:           1337,
			Outgoing_connections_count: 42,
			Incoming_connections_count: 42,
			White_peerlist_size:        42,
			Grey_peerlist_size:         42,
			Status:                     "OK",
		}

		json_reply, _ := json.Marshal(reply)

		fmt.Fprintf(w, string(json_reply))
	})

	log.Fatal(http.ListenAndServe(":18081", nil))

	fmt.Println("Exiting...")
}
