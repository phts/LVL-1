# NodeMCU firmware

## Commands

| Command          | `ok!`                | `fail!=<Description...>`                           | Description                                                                |
| ---------------- | -------------------- | -------------------------------------------------- | -------------------------------------------------------------------------- |
| `!connect`       | When connected       | -                                                  | Connect to WiFi. During connection sends responds like `progress!=<value>` |
| `!disconnect`    | Always               |                                                    | Disconnect WiFi                                                            |
| `!level=<value>` | On 2XX HTTP response | - When not connected<br>- On non-2XX HTTP response | Send level value to the website                                            |
| `!healthcheck`   | Always               | -                                                  | Respond with success at once                                               |
| `!fail=<desc>`   | -                    | Always                                             | Respond with failure at once (for testing)                                 |
| `!ping=<times>`  | Always               |                                                    | Send `pong!=<i>` <times> times with 10 sec delay                           |
