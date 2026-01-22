import { useState } from 'react';
import IkeaStyleHeader from './IkeaStyleHeader';
import IkeaStyleFooter from './IkeaStyleFooter';
import './IkeaContactPage.css';

const IkeaContactPage = ({ onNavigate }) => {
  const [formData, setFormData] = useState({
    name: '',
    email: '',
    subject: 'general',
    message: ''
  });

  const [errors, setErrors] = useState({});
  const [isSubmitting, setIsSubmitting] = useState(false);
  const [submitStatus, setSubmitStatus] = useState(null);

  const handleChange = (e) => {
    const { name, value } = e.target;
    setFormData(prev => ({
      ...prev,
      [name]: value
    }));
    // 清除该字段的错误
    if (errors[name]) {
      setErrors(prev => ({
        ...prev,
        [name]: ''
      }));
    }
  };

  const validate = () => {
    const newErrors = {};

    if (!formData.name.trim()) {
      newErrors.name = '姓名是必填项';
    }

    if (!formData.email.trim()) {
      newErrors.email = '邮箱是必填项';
    } else if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(formData.email)) {
      newErrors.email = '请输入有效的邮箱地址';
    }

    if (!formData.message.trim()) {
      newErrors.message = '留言内容是必填项';
    } else if (formData.message.trim().length < 10) {
      newErrors.message = '留言内容至少需要10个字符';
    }

    setErrors(newErrors);
    return Object.keys(newErrors).length === 0;
  };

  const handleSubmit = async (e) => {
    e.preventDefault();

    if (!validate()) {
      return;
    }

    setIsSubmitting(true);
    setSubmitStatus(null);

    try {
      // 模拟API调用
      await new Promise(resolve => setTimeout(resolve, 1500));
      
      setSubmitStatus('success');
      setFormData({
        name: '',
        email: '',
        subject: 'general',
        message: ''
      });
    } catch (error) {
      setSubmitStatus('error');
    } finally {
      setIsSubmitting(false);
      setTimeout(() => setSubmitStatus(null), 5000);
    }
  };

  const contactMethods = [
    {
      icon: (
        <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
          <path d="M4 4h16c1.1 0 2 .9 2 2v12c0 1.1-.9 2-2 2H4c-1.1 0-2-.9-2-2V6c0-1.1.9-2 2-2z"/>
          <polyline points="22,6 12,13 2,6"/>
        </svg>
      ),
      title: '邮箱联系',
      description: 'a993056494@163.com',
      action: 'mailto:a993056494@163.com'
    },
    {
      icon: (
        <svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor">
          <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z"/>
        </svg>
      ),
      title: 'GitHub',
      description: '查看我的开源项目',
      action: 'https://github.com/Shigure-moon'
    },
    {
      icon: (
        <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
          <path d="M21 15a2 2 0 0 1-2 2H7l-4 4V5a2 2 0 0 1 2-2h14a2 2 0 0 1 2 2z"/>
        </svg>
      ),
      title: '在线留言',
      description: '通过下方表单给我留言',
      action: '#contact-form'
    }
  ];

  return (
    <div className="ikea-contact">
      <IkeaStyleHeader 
        currentPage="contact" 
        onNavigate={onNavigate}
      />
      
      {/* Hero Section */}
      <section className="ikea-contact-hero">
        <div className="ikea-container">
          <div className="ikea-contact-hero-content">
            <h1 className="ikea-contact-title">联系我</h1>
            <p className="ikea-contact-subtitle">
              有任何问题或想法？我很乐意与您交流
            </p>
          </div>
        </div>
      </section>

      {/* Main Content */}
      <section className="ikea-contact-content">
        <div className="ikea-container">
          <div className="ikea-contact-grid">
            {/* 联系方式 */}
            <div className="ikea-contact-methods">
              <h2 className="ikea-section-title">联系方式</h2>
              <div className="ikea-contact-methods-grid">
                {contactMethods.map((method, index) => (
                  <a
                    key={index}
                    href={method.action}
                    target={method.action.startsWith('http') ? '_blank' : '_self'}
                    rel={method.action.startsWith('http') ? 'noopener noreferrer' : ''}
                    className="ikea-contact-method-card"
                    onClick={method.action.startsWith('#') ? (e) => {
                      e.preventDefault();
                      document.querySelector(method.action)?.scrollIntoView({ behavior: 'smooth' });
                    } : undefined}
                  >
                    <div className="ikea-contact-method-icon">
                      {method.icon}
                    </div>
                    <div className="ikea-contact-method-info">
                      <h3>{method.title}</h3>
                      <p>{method.description}</p>
                    </div>
                  </a>
                ))}
              </div>
              
              {/* 响应时间说明 */}
              <div className="ikea-contact-note">
                <div className="ikea-contact-note-icon">
                  <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                    <circle cx="12" cy="12" r="10"/>
                    <polyline points="12,6 12,12 16,14"/>
                  </svg>
                </div>
                <div className="ikea-contact-note-content">
                  <h4>响应时间</h4>
                  <p>我通常会在24小时内回复您的消息。如果是紧急事项，请通过邮箱联系我。</p>
                </div>
              </div>
            </div>

            {/* 联系表单 */}
            <div className="ikea-contact-form-section" id="contact-form">
              <h2 className="ikea-section-title">发送消息</h2>
              <div className="ikea-contact-form-wrapper">
                <form className="ikea-contact-form" onSubmit={handleSubmit} noValidate>
                  <div className="ikea-form-group">
                    <label htmlFor="name" className="ikea-form-label">
                      姓名 *
                    </label>
                    <input
                      type="text"
                      id="name"
                      name="name"
                      className={`ikea-form-input ${errors.name ? 'ikea-form-input--error' : ''}`}
                      placeholder="请输入您的姓名"
                      value={formData.name}
                      onChange={handleChange}
                      required
                    />
                    {errors.name && (
                      <p className="ikea-form-error">{errors.name}</p>
                    )}
                  </div>

                  <div className="ikea-form-group">
                    <label htmlFor="email" className="ikea-form-label">
                      邮箱地址 *
                    </label>
                    <input
                      type="email"
                      id="email"
                      name="email"
                      className={`ikea-form-input ${errors.email ? 'ikea-form-input--error' : ''}`}
                      placeholder="example@email.com"
                      value={formData.email}
                      onChange={handleChange}
                      required
                    />
                    {errors.email && (
                      <p className="ikea-form-error">{errors.email}</p>
                    )}
                  </div>

                  <div className="ikea-form-group">
                    <label htmlFor="subject" className="ikea-form-label">
                      主题
                    </label>
                    <select
                      id="subject"
                      name="subject"
                      className="ikea-form-select"
                      value={formData.subject}
                      onChange={handleChange}
                    >
                      <option value="general">一般咨询</option>
                      <option value="technical">技术交流</option>
                      <option value="collaboration">合作洽谈</option>
                      <option value="feedback">意见反馈</option>
                      <option value="other">其他</option>
                    </select>
                  </div>

                  <div className="ikea-form-group">
                    <label htmlFor="message" className="ikea-form-label">
                      留言内容 *
                    </label>
                    <textarea
                      id="message"
                      name="message"
                      className={`ikea-form-textarea ${errors.message ? 'ikea-form-textarea--error' : ''}`}
                      placeholder="请详细描述您的问题或想法..."
                      rows="6"
                      value={formData.message}
                      onChange={handleChange}
                      required
                    />
                    {errors.message && (
                      <p className="ikea-form-error">{errors.message}</p>
                    )}
                  </div>

                  {submitStatus === 'success' && (
                    <div className="ikea-form-success">
                      <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <polyline points="20,6 9,17 4,12"/>
                      </svg>
                      消息已成功发送！我会尽快回复您。
                    </div>
                  )}

                  {submitStatus === 'error' && (
                    <div className="ikea-form-error-message">
                      <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                        <circle cx="12" cy="12" r="10"/>
                        <line x1="15" y1="9" x2="9" y2="15"/>
                        <line x1="9" y1="9" x2="15" y2="15"/>
                      </svg>
                      发送失败，请稍后重试。
                    </div>
                  )}

                  <button
                    type="submit"
                    className="ikea-form-submit"
                    disabled={isSubmitting}
                  >
                    {isSubmitting ? (
                      <>
                        <div className="ikea-form-spinner"></div>
                        发送中...
                      </>
                    ) : (
                      <>
                        发送消息
                        <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                          <line x1="5" y1="12" x2="19" y2="12"/>
                          <polyline points="12,5 19,12 12,19"/>
                        </svg>
                      </>
                    )}
                  </button>
                </form>
              </div>
            </div>
          </div>
        </div>
      </section>

      <IkeaStyleFooter onNavigate={onNavigate} />
    </div>
  );
};

export default IkeaContactPage;