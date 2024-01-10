import asyncio
import aiohttp
import datetime

async def fetch_data(url):
    async with aiohttp.ClientSession() as session:
        async with session.get(url) as response:
            return await response.text()

async def main():
    a = datetime.datetime.now()
    result1 = await fetch_data("https://example.com")
    result2 = await fetch_data("https://example.org")
    print(result1, result2)
    b = datetime.datetime.now()
    delta = b - a
    print(delta)
asyncio.run(main())

