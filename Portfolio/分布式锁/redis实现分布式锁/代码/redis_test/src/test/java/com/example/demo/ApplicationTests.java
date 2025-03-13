package com.example.demo;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.test.context.junit4.*;
import com.redisson.lock.LockApplication;



@RunWith(SpringRunner.class)
@SpringBootTest(classes= {LockApplication.class}) //启动指定类
public class ApplicationTests {
	
	@Autowired
	StringRedisTemplate stringResdisTemplate;
	
	@Test
	public void test() throws Exception {
		//设置一个key，商品数量为10
		stringResdisTemplate.opsForValue().set("stock","10");
		Assert.assertEquals ("10", stringResdisTemplate.opsForValue().get("stock"));
	}
}