package com.redisson.lock;



import java.util.concurrent.TimeUnit;

import org.redisson.Redisson;
import org.redisson.api.RLock;
import org.redisson.config.Config;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.condition.ConditionalOnMissingBean;
import org.springframework.context.annotation.Bean;
import org.springframework.data.redis.connection.RedisConnectionFactory;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.data.redis.core.StringRedisTemplate;
import org.springframework.util.CollectionUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;



@RestController
@SpringBootApplication
public class LockApplication {
	
	
	@Autowired
	private Redisson redisson;
	@Autowired
	private StringRedisTemplate stringRedisTemplate;
	
	public static void main(String[] args) {
		SpringApplication.run(LockApplication.class,args);
		
	}
	
	@RequestMapping("/deduct_stock")
	public void deductStock() {
		String lockKey = "lockkey";
		try {
			// 得到锁
			Long lockvalue = stringRedisTemplate.opsForValue().increment(lockKey, 1);
			// 设置超时时间，估计业务执行时间不会超过10秒
			stringRedisTemplate.expire(lockKey, 10, TimeUnit.SECONDS);
			// 如果锁值为1，线程获得锁
			if(lockvalue==1) {
				
				// 获取stock键值
				int stock = Integer.parseInt(stringRedisTemplate.opsForValue().get("stock"));
				// 有库存则减一，报告库存数量
				if(stock>0) {
					stringRedisTemplate.opsForValue().set("stock", (stock-1)+"");
					System.out.println("扣减成功，库存stock："+(stock-1)+"");
				} else {
					System.out.println("扣减失败，库存不足");
				}
			}
		} finally {
			// 还原锁
			stringRedisTemplate.opsForValue().increment(lockKey, -1);
		}

		
//setnx方式进行分布锁
//		String key = "lockkey";
//		 redisLock lock = new redisLock(stringRedisTemplate, key, 10000, 20000);
//		 try {
//		            if(lock.lock()) {
//		            	int stock = Integer.parseInt(stringRedisTemplate.opsForValue().get("stock"));
//						if(stock>0)
//						{
//							stringRedisTemplate.opsForValue().set("stock",(stock - 1)+ "");
//							System.out.println("扣减成功，库存stock："+(stock - 1)+ "");
//							
//						}else
//						{
//							System.out.println("扣减失败。库存不足");
//						}
//		            }
//		            
//		        } catch (InterruptedException e) {
//		            e.printStackTrace();
//		        }finally {
//		            //为了让分布式锁的算法更稳键些，持有锁的客户端在解锁之前应该再检查一次自己的锁是否已经超时，再去做DEL操作，因为可能客户端因为某个耗时的操作而挂起，
//		            //操作完的时候锁因为超时已经被别人获得，这时就不必解锁了。 ————这里没有做
//		            lock.unlock();
//		        }
// redis的lock
//		String lockKey = "lockkey";
//		RLock lock = redisson.getLock(lockKey);
//		try {
//			
//			
//			lock.lock();
//			int stock = Integer.parseInt(stringRedisTemplate.opsForValue().get("stock"));
//				if(stock>0)
//				{
//					stringRedisTemplate.opsForValue().set("stock",(stock - 1)+ "");
//					System.out.println("扣减成功，库存stock："+(stock - 1)+ "");
//					
//				}else
//				{
//					System.out.println("扣减失败。库存不足");
//				}
//
//		} finally {
//			lock.unlock();
//		}
		
		
		
	}

	
	
	@Bean
	public Redisson redisson() {
		//单机模式
		Config config = new Config();
		config.useSingleServer().setAddress("redis://127.0.0.1:6379").setDatabase(0);
		return (Redisson) Redisson.create(config);
		
		}
	
	@Bean
	@ConditionalOnMissingBean(StringRedisTemplate.class)
	public StringRedisTemplate stringRedisTemplate(RedisConnectionFactory redisConnectionFactory) {
		StringRedisTemplate template = new StringRedisTemplate();
		template.setConnectionFactory(redisConnectionFactory);
		return template;
	}
	
}