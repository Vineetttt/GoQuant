import asyncio
import websockets
import json
import random
import time

class DeribitTestClient:
    def __init__(self, uri="ws://localhost:9002"):
        self.uri = uri
        self.websocket = None
        self.change_id = 1000
        self.last_bids = {}
        self.last_asks = {}

    async def connect(self):
        self.websocket = await websockets.connect(self.uri)
        print(f"Connected to {self.uri}")

    async def subscribe(self, instruments):
        for instrument in instruments:
            subscription_msg = {
                "jsonrpc": "2.0",
                "method": "public/subscribe",
                "id": 42,
                "params": {
                    "channels": [f"book.{instrument}.100ms"]
                }
            }
            await self.websocket.send(json.dumps(subscription_msg))
            print(f"Sent subscription request for {instrument}")

    def generate_price_levels(self, base_price, count=5):
        return [[base_price + (i * 0.5), round(random.uniform(1, 10), 2)] for i in range(count)]

    def create_snapshot(self, instrument):
        base_bid = random.uniform(40000, 41000)
        base_ask = base_bid + 10
        
        bids = self.generate_price_levels(base_bid, 5)
        asks = self.generate_price_levels(base_ask, 5)
        
        # Store current state
        self.last_bids = {price: amount for price, amount in bids}
        self.last_asks = {price: amount for price, amount in asks}
        
        snapshot = {
            "jsonrpc": "2.0",
            "method": "subscription",
            "params": {
                "channel": f"book.{instrument}.100ms",
                "data": {
                    "type": "snapshot",
                    "timestamp": int(time.time() * 1000),
                    "instrument_name": instrument,
                    "change_id": self.change_id,
                    "bids": [["new", price, amount] for price, amount in bids],
                    "asks": [["new", price, amount] for price, amount in asks]
                }
            }
        }

        print("-------------------")
        print("Sent initial snapshot:")
        print(json.dumps(snapshot, indent=4))
        print("-------------------")
        
        return snapshot

    def create_update(self, instrument):
        self.change_id += 1
        changes = []
        
        if self.last_bids:
            for price in random.sample(list(self.last_bids.keys()), 
                                    k=min(2, len(self.last_bids))):
                if random.random() < 0.3:  # 30% chance to delete
                    changes.append(("bids", ["delete", price, 0]))
                    del self.last_bids[price]
                else:
                    new_amount = round(random.uniform(1, 10), 2)
                    changes.append(("bids", ["change", price, new_amount]))
                    self.last_bids[price] = new_amount

        if self.last_asks:
            for price in random.sample(list(self.last_asks.keys()), 
                                    k=min(2, len(self.last_asks))):
                if random.random() < 0.3:  
                    changes.append(("asks", ["delete", price, 0]))
                    del self.last_asks[price]
                else:
                    new_amount = round(random.uniform(1, 10), 2)
                    changes.append(("asks", ["change", price, new_amount]))
                    self.last_asks[price] = new_amount

        update = {
            "jsonrpc": "2.0",
            "method": "subscription",
            "params": {
                "channel": f"book.{instrument}.100ms",
                "data": {
                    "type": "change",
                    "timestamp": int(time.time() * 1000),
                    "instrument_name": instrument,
                    "change_id": self.change_id,
                    "prev_change_id": self.change_id - 1,
                    "bids": [change for type_, change in changes if type_ == "bids"],
                    "asks": [change for type_, change in changes if type_ == "asks"]
                }
            }
        }

        print("-------------------")
        print("Sent order book update:")
        print(json.dumps(update, indent=4))
        print("-------------------")
        
        return update

    async def simulate_orderbook_updates(self, instrument):
        snapshot = self.create_snapshot(instrument)
        await self.websocket.send(json.dumps(snapshot))

        while True:
            await asyncio.sleep(10)  
            update = self.create_update(instrument)
            await self.websocket.send(json.dumps(update))

    async def receive_messages(self):
        while True:
            try:
                message = await self.websocket.recv()
                print("-------------------")
                print("Received message:")
                print(json.dumps(json.loads(message), indent=4))
                print("-------------------")
            except websockets.exceptions.ConnectionClosed:
                print("Connection closed")
                break

    async def run(self, instruments):
        await self.connect()
        await self.subscribe(instruments)

        tasks = [self.simulate_orderbook_updates(instrument) for instrument in instruments]
        await asyncio.gather(
            self.receive_messages(),
            *tasks
        )

async def main():
    with open('instruments.txt', 'r') as f:
        instruments = [line.strip() for line in f.readlines() if line.strip()]  

    client = DeribitTestClient()
    
    try:
        await client.run(instruments)
    except KeyboardInterrupt:
        print("\nClosing connection...")
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    asyncio.run(main())
